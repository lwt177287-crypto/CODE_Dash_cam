#common makefile header
#文字转语音
DIR_INC = ../../include
DIR_BIN = ../../bin
DIR_LIB = ../../libs

TARGET	= deepseek_iat_tts
BIN_TARGET = $(DIR_BIN)/$(TARGET)

CROSS_COMPILE = 
CFLAGS = -g -Wall -I$(DIR_INC)

ifdef LINUX64
LDFLAGS := -L$(DIR_LIB)/x64
else
LDFLAGS := -L$(DIR_LIB)/x86 
endif
LDFLAGS += -lmsc -lrt -ldl -lpthread -lasound -lstdc++
SRC=$(wildcard *.c) $(wildcard ../../src/*.c) $(wildcard ../iat_online_record_sample/*.c)
#将所有当前路径.c文件编译
OBJECTS := $(patsubst %.c,%.o,$(SRC))

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@
clean:
	@rm -f *.o ../../src/*.o ../iat_online_record_sample/*.o $(BIN_TARGET)

.PHONY:clean

#common makefile foot
