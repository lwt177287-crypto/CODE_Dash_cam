/*
 * 推流服务器：把摄像头实时画面通过 HTTP MJPEG 推给手机/电脑浏览器
 * 手机/电脑浏览器打开 http://<板子IP>:8080 即可实时观看
 * 独立线程运行，不影响板子 LCD 屏幕上的 LVGL 显示
 */

#include "video_car.h"

#define STREAM_PORT    8080            /* 手机/电脑访问的端口 */
#define MAX_JPEG_SIZE  (1024 * 1024)   /* 一帧 JPEG 最大 1MB */
#define STREAM_FPS     15              /* 推流帧率 */
#define JPEG_QUALITY   70              /* JPEG 压缩质量 1~100 */

/* mainloop 每采一帧就拷贝到这里（YUYV），推流线程从这里读 */
char g_stream_yuyv[IMAGE_WIDTH * IMAGE_HEIGHT * 2];

/* 最新一帧 JPEG（多个客户端共享） */
static unsigned char g_jpeg[MAX_JPEG_SIZE];
static int g_jpeg_len = 0;
static pthread_mutex_t g_jpeg_lock = PTHREAD_MUTEX_INITIALIZER;

/* ============ YUYV -> JPEG（输出到内存） ============ */
static int yuyv_to_jpeg_mem(const char *yuyv, int w, int h,
                            unsigned char *out, int out_max)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    unsigned char *rgb;
    int i, j, x;

    /* 1) YUYV -> RGB（复用项目里的 yuyv_to_rgb_pix） */
    rgb = (unsigned char *)malloc(w * h * 3);
    if (!rgb) return -1;

    for (i = 0, j = 0, x = 0; j < w * h; i += 4, j += 2, x += 6) {
        int argb1 = yuyv_to_rgb_pix(yuyv[i],     yuyv[i + 1], yuyv[i + 3]);
        int argb2 = yuyv_to_rgb_pix(yuyv[i + 2], yuyv[i + 1], yuyv[i + 3]);
        rgb[x + 0] = (unsigned char)(argb1 >> 16);
        rgb[x + 1] = (unsigned char)(argb1 >> 8);
        rgb[x + 2] = (unsigned char)(argb1);
        rgb[x + 3] = (unsigned char)(argb2 >> 16);
        rgb[x + 4] = (unsigned char)(argb2 >> 8);
        rgb[x + 5] = (unsigned char)(argb2);
    }

    /* 2) RGB -> JPEG（输出到内存） */
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &jpeg_buf, &jpeg_size);

    cinfo.image_width      = w;
    cinfo.image_height     = h;
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE);

    jpeg_start_compress(&cinfo, TRUE);
    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPROW row = (JSAMPROW)(rgb + cinfo.next_scanline * w * 3);
        jpeg_write_scanlines(&cinfo, &row, 1);
    }
    jpeg_finish_compress(&cinfo);

    if (jpeg_size > (unsigned long)out_max) {
        jpeg_destroy_compress(&cinfo);
        free(rgb);
        return -1;
    }
    memcpy(out, jpeg_buf, jpeg_size);
    jpeg_destroy_compress(&cinfo);
    free(rgb);
    return (int)jpeg_size;
}

/* ============ 发送完整数据（防止 send 只发一半） ============ */
static int send_all(int sock, const void *data, int len)
{
    const char *p = (const char *)data;
    int sent = 0;
    while (sent < len) {
        int r = send(sock, p + sent, len - sent, 0);
        if (r <= 0) return -1;
        sent += r;
    }
    return sent;
}

/* ============ 转换线程：YUYV -> JPEG，持续更新最新帧 ============ */
static void *convert_thread(void *arg)
{
    (void)arg;
    unsigned char *jpeg_tmp = malloc(MAX_JPEG_SIZE);
    char *yuyv_tmp = malloc(IMAGE_WIDTH * IMAGE_HEIGHT * 2);
    if (!jpeg_tmp || !yuyv_tmp) return NULL;

    while (1) {
        /* 从共享缓冲拷贝最新 YUYV（和 mainloop 用同一把锁） */
        pthread_mutex_lock(&mutex_reading_flgs);
        memcpy(yuyv_tmp, g_stream_yuyv, IMAGE_WIDTH * IMAGE_HEIGHT * 2);
        pthread_mutex_unlock(&mutex_reading_flgs);

        /* YUYV -> JPEG */
        int len = yuyv_to_jpeg_mem(yuyv_tmp, IMAGE_WIDTH, IMAGE_HEIGHT,
                                   jpeg_tmp, MAX_JPEG_SIZE);
        if (len > 0) {
            pthread_mutex_lock(&g_jpeg_lock);
            memcpy(g_jpeg, jpeg_tmp, len);
            g_jpeg_len = len;
            pthread_mutex_unlock(&g_jpeg_lock);
        }

        usleep(1000000 / STREAM_FPS);
    }
    return NULL;
}

/* ============ 客户端线程：MJPEG 推送 ============ */
static void *client_thread(void *arg)
{
    int sock = (int)(long)arg;
    char req[1024];
    int n = recv(sock, req, sizeof(req) - 1, 0);
    if (n <= 0) { close(sock); return NULL; }
    req[n] = 0;

    if (strstr(req, "/stream")) {
        char hdr[256];
        int hl = sprintf(hdr,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
            "Cache-Control: no-cache\r\n\r\n");
        send_all(sock, hdr, hl);

        unsigned char *tmp = malloc(MAX_JPEG_SIZE);
        while (tmp) {
            pthread_mutex_lock(&g_jpeg_lock);
            int len = g_jpeg_len;
            memcpy(tmp, g_jpeg, len);
            pthread_mutex_unlock(&g_jpeg_lock);

            char part[128];
            int pl = sprintf(part,
                "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", len);
            if (send_all(sock, part, pl) < 0) break;
            if (send_all(sock, tmp, len) < 0)  break;
            if (send_all(sock, "\r\n", 2) < 0) break;

            usleep(1000000 / STREAM_FPS);
        }
        free(tmp);
    } else {
        const char *html =
            "<html><head><meta charset='utf-8'><title>行车记录仪</title></head>"
            "<body style='margin:0;background:#000'>"
            "<img src='/stream' style='width:100%;height:auto'/>"
            "</body></html>";
        char hdr[256];
        int hl = sprintf(hdr,
            "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n"
            "Content-Length: %d\r\n\r\n", (int)strlen(html));
        send_all(sock, hdr, hl);
        send_all(sock, html, strlen(html));
    }

    close(sock);
    return NULL;
}

/* ============ 推流服务器线程入口 ============ */
void *stream_server_thread(void *arg)
{
    (void)arg;
    int server, client;
    struct sockaddr_in addr;
    int one = 1;
    pthread_t ct;

    /* 先启动转换线程 */
    pthread_create(&ct, NULL, convert_thread, NULL);
    pthread_detach(ct);

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) { perror("推流 socket 失败"); return NULL; }
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(STREAM_PORT);
    if (bind(server, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("推流端口绑定失败");
        return NULL;
    }
    listen(server, 10);
    printf("推流服务器已启动，浏览器访问 http://<板子IP>:%d\n", STREAM_PORT);

    while (1) {
        client = accept(server, NULL, NULL);
        if (client < 0) continue;
        pthread_create(&ct, NULL, client_thread, (void *)(long)client);
        pthread_detach(ct);
    }
    return NULL;
}
