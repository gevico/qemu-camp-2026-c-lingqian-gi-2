#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_STACK 100

// 操作数栈
double num_stack[MAX_STACK];
int num_top = -1;

// 运算符栈
char op_stack[MAX_STACK];
int op_top = -1;

// 入栈：操作数
void push_num(double num) {
    if (num_top >= MAX_STACK - 1) {
        perror("num stack overflow");
        exit(1);
    }
    num_stack[++num_top] = num;
}

// 出栈：操作数
double pop_num() {
    if (num_top < 0) {
        perror("num stack underflow");
        exit(1);
    }
    return num_stack[num_top--];
}

// 入栈：运算符
void push_op(char op) {
    if (op_top >= MAX_STACK - 1) {
        perror("op stack overflow");
        exit(1);
    }
    op_stack[++op_top] = op;
}

// 出栈：运算符
char pop_op() {
    if (op_top < 0) {
        perror("op stack underflow");
        exit(1);
    }
    return op_stack[op_top--];
}

// 获取栈顶运算符
char peek_op() {
    if (op_top < 0) return '\0';
    return op_stack[op_top];
}

// 运算符优先级：* / 优先级2，+ - 优先级1，( 优先级0
int op_priority(char op) {
    switch (op) {
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        case '(': return 0;
        default: return -1;
    }
}

// 计算两个数的运算
double calc(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (fabs(b) < 1e-6) {
                perror("divide by zero");
                exit(1);
            }
            return a / b;
        default: 
            perror("invalid operator");
            exit(1);
    }
}

// 核心解析函数
double calculate(const char *expr) {
    int i = 0;
    int len = strlen(expr);
    while (i < len) {
        // 跳过空格
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        // 数字：拼接多位数/小数
        if (isdigit(expr[i]) || expr[i] == '.') {
            double num = 0.0;
            int decimal = 0;
            double scale = 1.0;
            // 整数部分
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            // 小数部分
            if (expr[i] == '.') {
                i++;
                decimal = 1;
                while (isdigit(expr[i])) {
                    scale /= 10;
                    num += (expr[i] - '0') * scale;
                    i++;
                }
            }
            push_num(num);
        }
        // 左括号：直接入栈
        else if (expr[i] == '(') {
            push_op(expr[i]);
            i++;
        }
        // 右括号：计算到左括号
        else if (expr[i] == ')') {
            while (peek_op() != '(') {
                char op = pop_op();
                double b = pop_num();
                double a = pop_num();
                push_num(calc(a, b, op));
            }
            pop_op(); // 弹出左括号
            i++;
        }
        // 运算符：按优先级处理
        else if (strchr("+-*/", expr[i])) {
            while (op_top >= 0 && op_priority(peek_op()) >= op_priority(expr[i])) {
                char op = pop_op();
                double b = pop_num();
                double a = pop_num();
                push_num(calc(a, b, op));
            }
            push_op(expr[i]);
            i++;
        }
        // 非法字符
        else {
            fprintf(stderr, "invalid character: %c\n", expr[i]);
            exit(1);
        }
    }

    // 处理剩余运算符
    while (op_top >= 0) {
        char op = pop_op();
        double b = pop_num();
        double a = pop_num();
        push_num(calc(a, b, op));
    }

    return pop_num();
}

// 主函数：命令行输入表达式
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"expression\"\n", argv[0]);
        fprintf(stderr, "Example: %s \"(1+2)*3-4/2\"\n", argv[0]);
        return 1;
    }

    double result = calculate(argv[1]);
    printf("Result: %.2f\n", result);
    return 0;
}
