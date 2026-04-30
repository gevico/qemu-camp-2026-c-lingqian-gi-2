#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    // 定位第一个 '/' 之后的位置（即旧字符串的起始位置）
    const char* start = cmd + 2;
    const char* end = strchr(start, '/');
    if (end == NULL) {
        return -1;
    }

    // 提取旧字符串
    size_t old_len = end - start;
    *old_str = (char*)malloc(old_len + 1);
    if (*old_str == NULL) return -1;
    strncpy(*old_str, start, old_len);
    (*old_str)[old_len] = '\0';

    // 定位新字符串的起始位置
    start = end + 1;
    end = strchr(start, '/');
    if (end == NULL) {
        free(*old_str);
        return -1;
    }

    // 提取新字符串
    size_t new_len = end - start;
    *new_str = (char*)malloc(new_len + 1);
    if (*new_str == NULL) {
        free(*old_str);
        return -1;
    }
    strncpy(*new_str, start, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char* pos = strstr(str, old);
    if (pos == NULL) {
        return; // 未找到匹配，直接返回
    }

    // 计算各部分的长度
    size_t prefix_len = pos - str;
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t suffix_len = strlen(pos + old_len);

    // 确保新字符串不超过缓冲区大小
    if (prefix_len + new_len + suffix_len >= MAX_LINE_LENGTH) {
        // 长度溢出，不执行替换（也可考虑其他处理方式）
        return;
    }

    // 构建新字符串
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, str, prefix_len);
    temp[prefix_len] = '\0';
    strcat(temp, new);
    strcat(temp, pos + old_len);

    // 复制回原字符串
    strcpy(str, temp);
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";
    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};
    char* old_str = NULL;
    char* new_str = NULL;

    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
