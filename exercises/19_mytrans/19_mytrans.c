// 19_mytrans.c 主程序
#include "myhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 移除单词中的标点符号（仅保留字母/数字/点号）
void remove_punctuation(char *word) {
    if (!word)
        return;
    char *dst = word;
    for (char *src = word; *src; src++) {
        if (isalnum((unsigned char)*src) || *src == '.') {
            *dst++ = *src;
        }
    }
    *dst = '\0';
}

int main() {
    // 1. 创建哈希表
    HashTable *table = create_hash_table();
    if (!table) {
        fprintf(stderr, "无法创建哈希表\n");
        return 1;
    }

    // 2. 加载词典
    uint64_t dict_count = 0;
    if (load_dictionary("dict.txt", table, &dict_count) != 0) {
        fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
        free_hash_table(table);
        return 1;
    }
    printf("词典加载完成，共计%ld词条。\n", dict_count);

    // 3. 读取text.txt文件并处理单词
    FILE *file = fopen("text.txt", "r");
    if (!file) {
        perror("无法打开text.txt文件");
        free_hash_table(table);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        // 按空格分割单词
        char *word = strtok(line, " ");
        while (word != NULL) {
            // 清理单词（去标点、转小写、修剪）
            char clean_word[100] = {0};
            strncpy(clean_word, word, sizeof(clean_word) - 1);
            remove_punctuation(clean_word);
            to_lowercase(clean_word);
            trim(clean_word);

            if (strlen(clean_word) == 0) {
                word = strtok(NULL, " ");
                continue;
            }

            // 查找翻译
            const char *translation = hash_table_lookup(table, clean_word);
            
            // 严格按测试要求输出格式
            printf("原文: %s\t", clean_word);
            if (translation) {
                printf("翻译: %s\n", translation);
            } else {
                printf("未找到该单词的翻译。\n");
            }

            word = strtok(NULL, " ");
        }
    }

    // 4. 释放资源
    fclose(file);
    free_hash_table(table);
    return 0;
}
