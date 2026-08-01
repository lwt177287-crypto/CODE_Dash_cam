cmake_minimum_required(VERSION 3.5)#版本

project(att)    #项目名

#头文件路径
include_directories(
./include
)
#执行文件名
set(TARGET main)

#需要的.c
set(
SRCS
src/Album.c
src/child.c
src/ent0.c
src/jly.c
src/JPEG-C.c
src/JPEG-X.c
src/video_car.c
src/v_c_Exe_main.c
src/settings.c
src/sms.c
src/key_board.c


)
#添加目标和依赖
add_executable(
${TARGET}
${SRCS}
)   
#指定库路径
target_link_directories(${TARGET}
    PRIVATE
        ./lib
        )

#指定库
        target_link_libraries(
                              ${TARGET}
                                jpeg
                                pthread
                                )

