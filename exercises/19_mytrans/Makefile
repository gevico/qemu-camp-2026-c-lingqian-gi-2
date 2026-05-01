CC = gcc
CFLAGS = -Wall -g -O2

TARGET = 19_mytrans
SRCS = 19_mytrans.c mytrans.c myhash.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

# 补全编译规则：链接目标文件生成可执行程序
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 补全编译规则：编译.c文件为.o文件
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
