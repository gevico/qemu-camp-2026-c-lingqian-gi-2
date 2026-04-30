#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    // 检查输入URL是否为空
    if (url == NULL || strlen(url) == 0) {
        err = EINVAL;
        goto exit;
    }

    // 1. 找到URL中?的位置（查询参数起始位置）
    const char* query_start = strchr(url, '?');
    if (query_start == NULL) {
        // 没有查询参数，正常返回
        goto exit;
    }
    query_start++; // 跳过?，指向查询参数起始

    // 2. 复制查询字符串（strtok会修改原字符串，需拷贝）
    size_t query_len = strlen(query_start);
    char* query_str = (char*)malloc(query_len + 1);
    if (query_str == NULL) {
        err = ENOMEM;
        goto exit;
    }
    strcpy(query_str, query_start);

    // 3. 按&分割每个键值对
    char* pair = strtok(query_str, "&");
    while (pair != NULL) {
        // 4. 按=分割key和value
        char* equal_sign = strchr(pair, '=');
        if (equal_sign != NULL) {
            // 分割key和value
            *equal_sign = '\0'; // 将=替换为字符串结束符
            char* key = pair;
            char* value = equal_sign + 1;

            // 5. 处理value中的+号（替换为空格）
            char* plus = value;
            while ((plus = strchr(plus, '+')) != NULL) {
                *plus = ' ';
                plus++;
            }

            // 6. 按指定格式输出键值对
            printf("key = %s, value = %s\n", key, value);
        }

        // 继续分割下一个键值对
        pair = strtok(NULL, "&");
    }

    // 释放内存
    free(query_str);

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}
