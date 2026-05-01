CC = gcc
CFLAGS = -Wall -g -O2
TARGET = 19_mytrans
SRCS = 19_mytrans.c mytrans.c myhash.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

# 编译目标，链接所有目标文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 自动生成依赖规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
