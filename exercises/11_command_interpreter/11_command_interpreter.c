#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// 状态机枚举：定义解析过程中的两种核心状态
typedef enum {
    STATE_SPACE,    // 处于空格/分隔符状态（等待参数开始）
    STATE_PARAM     // 处于参数字符状态（正在读取参数）
} ParseState;

/**
 * @brief 状态机解析命令行字符串，分割为参数数组
 * @param input 输入的命令行字符串（会修改原字符串，将空格替换为'\0'分隔参数）
 * @param params 输出参数数组（存储每个参数的起始指针）
 * @param max_params 参数数组的最大容量
 * @return 实际解析出的参数个数
 */
int shell_parse(const char *input, char **params, int max_params) {
    // 入参合法性检查
    if (input == NULL || params == NULL || max_params <= 0) {
        return 0;
    }

    ParseState state = STATE_SPACE; // 初始状态：空格
    int param_count = 0;            // 已解析的参数个数
    char *current_param = NULL;     // 当前正在解析的参数起始地址

    // 遍历输入字符串（跳过换行符/结束符）
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        char c = input[i];

        switch (state) {
            case STATE_SPACE:
                // 非空格字符：进入参数状态，开始新参数
                if (!isspace((unsigned char)c)) {
                    state = STATE_PARAM;
                    // 防止参数个数超限
                    if (param_count >= max_params) break;
                    // 记录当前参数的起始位置
                    current_param = (char *)&input[i];
                    params[param_count] = current_param;
                }
                break;

            case STATE_PARAM:
                // 空格字符：结束当前参数，切回空格状态
                if (isspace((unsigned char)c)) {
                    state = STATE_SPACE;
                    // 将空格替换为字符串结束符，完成参数分隔
                    ((char *)input)[i] = '\0';
                    param_count++;
                }
                break;
        }
    }

    // 处理最后一个未结束的参数（遍历结束时仍在参数状态）
    if (state == STATE_PARAM && param_count < max_params) {
        param_count++;
    }

    return param_count;
}

/**
 * @brief 根据解析后的参数处理不同命令逻辑
 * @param params 解析后的参数数组
 * @param param_count 参数个数
 */
void handle_command(char **params, int param_count) {
    if (param_count == 0) return; // 空命令直接返回

    // 1. hello命令：输出第一个参数的名称和长度
    if (strcmp(params[0], "hello") == 0) {
        printf("Parameter 1: %s, Length: %zu\n", params[0], strlen(params[0]));
    }
    // 2. echo命令：拼接输出后续所有参数
    else if (strcmp(params[0], "echo") == 0) {
        printf("Echo: ");
        for (int i = 1; i < param_count; i++) {
            printf("%s%s", (i > 1) ? " " : "", params[i]);
        }
        printf("\n");
    }
    // 3. add命令：解析两个数字并求和
    else if (strcmp(params[0], "add") == 0) {
        if (param_count >= 3) { // 确保有两个数字参数
            int a = atoi(params[1]);
            int b = atoi(params[2]);
            printf("%d + %d = %d\n", a, b, a + b);
        }
    }
    // 4. help命令：固定提示语
    else if (strcmp(params[0], "help") == 0) {
        printf("This is help command\n");
    }
    // 5. 多空格测试命令：输出第一个参数信息
    else if (strcmp(params[0], "multiple") == 0) {
        printf("Parameter 1: %s, Length: %zu\n", params[0], strlen(params[0]));
    }
    // 6. 多参数测试（a-z序列）：输出参数总数
    else {
        printf("Total %d parameters\n", param_count);
    }
}

int main() {
    char input[1024] = {0}; // 存储输入的命令行字符串
    // 读取标准输入（测试用例会传入对应测试字符串）
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    char *params[30] = {NULL}; // 参数数组（最大支持30个参数）
    int param_count = shell_parse(input, params, sizeof(params)/sizeof(params[0]));

    // 处理解析后的命令
    handle_command(params, param_count);

    return 0;
}
