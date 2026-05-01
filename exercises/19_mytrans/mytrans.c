// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 补全：去除字符串首尾空白字符（空格、制表符、换行、回车等）
void trim(char *str) {
    if (!str || strlen(str) == 0)
        return;

    // 去除尾部空白
    int end = strlen(str) - 1;
    while (end >= 0 && isspace((unsigned char)str[end])) {
        end--;
    }
    str[end + 1] = '\0';

    // 去除头部空白
    int start = 0;
    while (start < end && isspace((unsigned char)str[start])) {
        start++;
    }

    // 移动字符串到头部
    if (start > 0) {
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
    }
}

// 补全：加载词典文件到哈希表
int load_dictionary(const char *filename, HashTable *table, uint64_t *dict_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开词典文件");
        return -1;
    }

    char line[1024];
    char current_word[100] = {0};
    char current_translation[1024] = {0};
    int in_entry = 0;
    *dict_count = 0; // 初始化计数

    // 逐行读取词典文件
    while (fgets(line, sizeof(line), file) != NULL) {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';
        trim(line); // 修剪首尾空白

        if (strlen(line) == 0)
            continue;

        // 分割单词和翻译（按第一个制表符/空格分割）
        char *sep = strchr(line, '\t');
        if (!sep) {
            sep = strchr(line, ' ');
            if (!sep)
                continue; // 无效行，跳过
        }

        // 提取单词（小写）和翻译
        int word_len = sep - line;
        if (word_len <= 0 || word_len >= sizeof(current_word))
            continue;

        strncpy(current_word, line, word_len);
        current_word[word_len] = '\0';
        trim(current_word);
        to_lowercase(current_word); // 转换为小写，保证查找不区分大小写

        // 提取翻译
        strncpy(current_translation, sep + 1, sizeof(current_translation) - 1);
        current_translation[sizeof(current_translation) - 1] = '\0';
        trim(current_translation);

        // 插入哈希表
        if (hash_table_insert(table, current_word, current_translation)) {
            (*dict_count)++; // 计数+1
        }
    }

    fclose(file);
    return 0;
}

// 转换字符串为小写
void to_lowercase(char *str) {
    for (; *str; ++str)
        *str = tolower((unsigned char)*str);
}
