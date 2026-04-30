#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */
int parse_url(const char* url) {
    int err = 0;

    // 查找问号，定位查询字符串起始位置
    const char* query = strchr(url, '?');
    if (query == NULL) {
        // 没有查询参数，直接返回
        return err;
    }
    query++; // 跳过 '?'

    // 复制查询字符串（使用标准C函数替代 strdup）
    size_t len = strlen(query);
    char* query_copy = (char*)malloc(len + 1);
    if (query_copy == NULL) {
        return -1;
    }
    strcpy(query_copy, query);

    // 使用 strtok 按 '&' 分割参数
    char* token = strtok(query_copy, "&");
    while (token != NULL) {
        // 查找 '=' 分割键和值
        char* equal_sign = strchr(token, '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0';  // 将键值对分隔开
            char* key = token;
            char* value = equal_sign + 1;
            printf("key = %s, value = %s\n", key, value);
        }
        token = strtok(NULL, "&");
    }

    free(query_copy);
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";
    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");
    parse_url(test_url);
    return 0;
}
