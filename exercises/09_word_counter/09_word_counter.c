#include <stdio.h>
#include <string.h>

int main() {
    // 测试指定的目标字符串
    char target_str[] = "Don't ask what your country can do for you, but ask what you can do for your country.";
    int word_count = 0;    // 单词计数器
    int in_word = 0;       // 状态变量：0=不在单词中，1=在单词中

    // 核心：遍历字符串统计单词
    for (int i = 0; target_str[i] != '\0'; i++) {
        // 情况1：当前字符非空格，且未处于单词中 → 新单词开始
        if (target_str[i] != ' ' && target_str[i] != '\n' && !in_word) {
            word_count++;
            in_word = 1; // 标记进入单词
        }
        // 情况2：当前字符是空格 → 离开单词
        else if (target_str[i] == ' ' || target_str[i] == '\n') {
            in_word = 0; // 标记退出单词
        }
        // 情况3：当前字符非空格且已处于单词中 → 无操作（继续在单词内）
    }

    // 输出格式严格匹配测试用例的解析规则（"单词数量: %d"）
    printf("单词数量: %d\n", word_count);

    return 0;
}
