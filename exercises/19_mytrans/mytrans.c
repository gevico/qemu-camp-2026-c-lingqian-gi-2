// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 去除字符串首尾空白字符（空格、制表符、换行符等）
void trim(char *str) {
  if (!str || *str == '\0')
    return;

  // 跳过开头空白
  char *start = str;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }

  // 移动有效字符到开头
  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }

  // 处理结尾空白
  char *end = str + strlen(str) - 1;
  while (end >= str && isspace((unsigned char)*end)) {
    end--;
  }
  *(end + 1) = '\0';
}

// 加载词典文件到哈希表（dict.txt格式：每行 "单词 翻译内容"）
int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  *dict_count = 0;

  // 逐行读取词典
  while (fgets(line, sizeof(line), file) != NULL) {
    // 去除换行符和首尾空白
    line[strcspn(line, "\n")] = '\0';
    trim(line);

    if (strlen(line) == 0)
      continue;

    // 分割单词和翻译（第一个空格为分隔符）
    char *sep = strchr(line, ' ');
    if (!sep)
      continue;

    // 拆分key和value
    *sep = '\0';
    char *word = line;
    char *translation = sep + 1;
    trim(word);
    trim(translation);

    // 插入哈希表
    if (hash_table_insert(table, word, translation)) {
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}
