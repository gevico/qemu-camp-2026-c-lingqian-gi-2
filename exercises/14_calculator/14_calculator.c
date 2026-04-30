#include <stdio.h>
#include <stdlib.h> // 用于exit处理除零错误

// 实现四则运算函数
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    // 处理除零错误
    if (b == 0) {
        printf("错误：除数不能为零\n");
        exit(1); // 退出程序，也可返回特定值，需保证测试能检测到错误信息
    }
    return a / b;
}

int main() {
    int a, b;
    char op;

    // 定义函数指针数组：类型为int (*)(int, int)，数组名operations
    int (*operations[])(int, int) = {add, subtract, multiply, divide};
    char operators[] = "+-*/";

    printf("输入两个整数和一个运算符 (+, -, *, /): ");
    scanf("%d %d %c", &a, &b, &op);

    // 查找对应的运算符索引
    int index = -1;
    for (int i = 0; i < 4; i++) {
        if (op == operators[i]) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("无效的运算符\n");
        return 1;
    }

    int result = operations[index](a, b);
    printf("%d %c %d = %d\n", a, op, b, result);

    return 0;
}
