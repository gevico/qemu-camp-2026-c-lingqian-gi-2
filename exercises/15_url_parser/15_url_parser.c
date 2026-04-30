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
    char *token = NULL;
    char *key = NULL;
    char *value = NULL;

    // 检查输入URL是否为空
    if (url == NULL) {
        err = EINVAL;
        goto exit;
    }

    // 找到URL中参数部分的起始位置（?后面）
    param_start = strchr(url, '?');
    if (param_start == NULL) {
        // 没有参数部分，直接返回
        err = 0;
        goto exit;
    }
    param_start++; // 跳过?，指向参数起始位置

    // 复制参数字符串（strtok会修改原字符串，原URL是const不可修改）
    params = strdup(param_start);
    if (params == NULL) {
        err = ENOMEM;
        goto exit;
    }

    // 按&分割参数，遍历所有key-value对
    token = strtok(params, "&");
    while (token != NULL) {
        // 按=分割key和value
        key = strtok(token, "=");
        value = strtok(NULL, "="); // value可能为空，需处理

        // 输出键值对（兼容value为空的情况）
        if (key != NULL) {
            if (value == NULL) {
                value = "";
            }
            printf("key = %s, value = %s\n", key, value);
        }

        // 继续解析下一个参数
        token = strtok(NULL, "&");
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
