#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 10
#define MAX_INPUT_LENGTH 256

int shell_parse(char *buf, char *argv[]);
void execute_command(int argc, char *argv[]);

// in:  format -> hello world "hello world" 1234567
// Multiple command parameters entered from the command line (number of parameters does not exceed 10)
// out:  format -> Parameter X: Content, Length: X
// Parse the command parameters through a character pointer array, and sequentially display the content and length of each parameter on the screen
//
int main(void)
{
    FILE *file;
    char input[MAX_INPUT_LENGTH];
    char *argv[MAX_ARGS + 1] = {NULL};
    int argc = 0;

    file = fopen("command_file.txt", "r");
    if (!file) {
        fprintf(stderr, "❌ Error: Cannot open input file '%s'\n", "command_file.txt");
        return 1;
    }

    printf("✅ Reading commands from '%s':\n\n", "command_file.txt");

    while (fgets(input, MAX_INPUT_LENGTH, file) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0 || strspn(input, " \t") == strlen(input)) {
            continue;
        }

        printf("➡️  Input: %s\n", input);

        argc = shell_parse(input, argv);

        if (argc == 0) {
            printf("⚠️  No valid command parsed.\n\n");
            continue;
        }

        execute_command(argc, argv);
        printf("\n");
    }

    fclose(file);
    return 0;
}

// shell_parse 和 execute_command 保持不变
int shell_parse(char *buf, char *argv[])
{
    int argc = 0;
    int state = 0; // 状态机：0=等待参数开始，1=读取参数内容
    int i = 0;

    // 清空argv数组，防止残留指针导致异常
    for (int j = 0; j < MAX_ARGS + 1; j++) {
        argv[j] = NULL;
    }

    // 状态机核心逻辑：遍历字符串解析参数
    while (buf[i] != '\0' && argc < MAX_ARGS) {
        if (state == 0) {
            // 初始状态：跳过空格/制表符，找到参数起始位置
            if (buf[i] != ' ' && buf[i] != '\t') {
                argv[argc] = &buf[i]; // 标记当前参数的起始地址
                argc++;              // 参数计数+1
                state = 1;           // 切换到"读取参数内容"状态
            }
        } else {
            // 读取参数状态：遇到空格/制表符则终止当前参数
            if (buf[i] == ' ' || buf[i] == '\t') {
                buf[i] = '\0';       // 将分隔符替换为字符串结束符
                state = 0;           // 切回"等待参数开始"状态
            }
        }
        i++; // 遍历下一个字符
    }

    // 确保最后一个参数正确终止（若字符串结束时仍在读取参数状态）
    // 补充：argv最后一个元素置NULL（符合命令行参数惯例）
    argv[argc] = NULL;

    return argc;
}

void execute_command(int argc, char *argv[])
{
    printf("Parsing result: Total %d parameters\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Parameter %d: Content: %s, Length: %zu\n",
               i + 1, argv[i], strlen(argv[i]));
    }

    if (strcmp(argv[0], "help") == 0) {
        printf("This is help command\n");
    } else if (strcmp(argv[0], "echo") == 0) {
        printf("Echo: ");
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    } else if (strcmp(argv[0], "add") == 0 && argc == 3) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        printf("%d + %d = %d\n", a, b, a + b);
    } else {
        printf("Unknown command: %s\n", argv[0]);
    }
}
