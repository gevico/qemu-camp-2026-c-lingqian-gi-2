CC = gcc
CFLAGS = -Wall -g -O2

TARGET := 16_mysed
SRCS := 16_mysed.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

# 编译可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 编译目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
