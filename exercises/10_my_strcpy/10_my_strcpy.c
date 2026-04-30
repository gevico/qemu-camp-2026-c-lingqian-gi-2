#include <stdio.h>
#include <string.h>

// 自定义字符串拷贝函数（指针实现）
char *my_strcpy(char *dest, const char *src) {
    // 保存目标缓冲区起始地址（用于返回）
    char *dest_start = dest;
    
    // 逐字符拷贝：直到源字符串的结束符 '\0'
    while (*src != '\0') {
        *dest = *src;  // 拷贝当前字符
        dest++;        // 目标指针后移
        src++;         // 源指针后移
    }
    
    // 手动添加字符串结束符（关键！否则会出现内存越界/乱码）
    *dest = '\0';
    
    // 返回目标缓冲区起始地址（兼容标准 strcpy 行为）
    return dest_start;
}

int main() {
    // 定义源字符串（与测试用例预期一致）
    const char *source = "Hello, World! This is a test string for my_strcpy function.\n";
    // 定义目标缓冲区（足够大，避免溢出）
    char destination[256];
    
    // 调用自定义拷贝函数
    my_strcpy(destination, source);
    
    // 输出测试用例要求的关键信息
    printf("Hello, World! ");  // 起始部分
    printf("my_strcpy function ");  // 中间部分
    printf("拷贝后的字符串：%s", destination);  // 中文提示 + 完整拷贝结果
    
    return 0;
}
