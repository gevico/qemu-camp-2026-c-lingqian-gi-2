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
    char *param_start = NULL;
    char *params = NULL;
    char *pair = NULL;
    char *key = NULL;
    char *value = NULL;

    // 检查输入URL是否为空
    if (url == NULL || strlen(url) == 0) {
        err = EINVAL;
        goto exit;
    }

    // 找到URL中参数部分的起始位置（?后面）
    param_start = strchr(url, '?');
    if (param_start == NULL) {
        err = EINVAL; // 无参数部分
        goto exit;
    }
    param_start++; // 跳过?符号，指向参数起始位置

    // 复制参数部分（strtok会修改原字符串，需拷贝）
    params = strdup(param_start);
    if (params == NULL) {
        err = ENOMEM; // 内存分配失败
        goto exit;
    }

    // 按&分割每个键值对
    pair = strtok(params, "&");
    while (pair != NULL) {
        // 按=分割key和value
        key = strtok(pair, "=");
        value = strtok(NULL, "=");
        
        // 确保key和value都有效
        if (key != NULL && value != NULL) {
            printf("key = %s, value = %s\n", key, value);
        }

        // 继续解析下一个键值对
        pair = strtok(NULL, "&");
    }

exit:
    // 释放内存，避免泄漏
    if (params != NULL) {
        free(params);
    }
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}
