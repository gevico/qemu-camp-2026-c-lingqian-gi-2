// mytrans.c
#include "myhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

void trim(char *str) {
    if (str == NULL || *str == '\0') return;
    // 去尾部空白
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    // 去首部空白
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int load_dictionary(const char *filename, HashTable *table, uint64_t *dict_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开词典文件");
        return -1;
    }

    char line[1024];
    char word[256] = "";    // 暂存当前单词
    int have_word = 0;      // 是否已读取到单词行

    while (fgets(line, sizeof(line), file) != NULL) {
        // 去掉换行符
        line[strcspn(line, "\n")] = '\0';

        // 跳过纯空行
        if (line[0] == '\0')
            continue;

        // 如果本行是单词行（以 # 开头）
        if (line[0] == '#') {
            // 提取单词：# 后面的部分
            char *w = line + 1;
            trim(w);
            if (strlen(w) > 0) {
                strncpy(word, w, sizeof(word) - 1);
                word[sizeof(word) - 1] = '\0';
                have_word = 1;   // 标记已读取到单词
            }
            continue;
        }

        // 如果本行是翻译行（以 Trans: 开头）
        if (have_word && strncmp(line, "Trans:", 6) == 0) {
            char *trans = line + 6;   // 跳过 "Trans:"
            trim(trans);
            if (strlen(trans) > 0 && strlen(word) > 0) {
                hash_table_insert(table, word, trans);
                if (dict_count) (*dict_count)++;
            }
            have_word = 0;   // 重置，等待下一个词条
            word[0] = '\0';
        }
        // 其他情况（如注释行或格式不正确的行）直接忽略
    }

    fclose(file);
    return 0;
}
