/**
 * 06_stack_maze.c
 * 迷宫求解（深度优先搜索DFS + 栈实现）
 * 功能：找到5x5迷宫中从(0,0)到(4,4)的路径，并打印路径坐标（终点→起点）
 * 迷宫规则：0=通路，1=墙壁；路径要求连续（相邻坐标仅单行/列差1）
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// 迷宫尺寸定义
#define MAX_ROW 5
#define MAX_COL 5
#define MAX_STACK_SIZE 100  // 栈的最大容量（足够存储迷宫路径）

// 坐标结构体：存储迷宫中的行列位置
typedef struct {
    int row;  // 行
    int col;  // 列
} Coord;

// 栈结构体：用于DFS的路径存储（后进先出）
typedef struct {
    Coord data[MAX_STACK_SIZE];
    int top;  // 栈顶指针（-1表示空栈）
} Stack;

// ===================== 栈操作函数 =====================
// 初始化栈
void stack_init(Stack *stack) {
    stack->top = -1;
}

// 判断栈是否为空
bool stack_is_empty(Stack *stack) {
    return stack->top == -1;
}

// 判断栈是否已满
bool stack_is_full(Stack *stack) {
    return stack->top == MAX_STACK_SIZE - 1;
}

// 压栈：将坐标存入栈
bool stack_push(Stack *stack, Coord coord) {
    if (stack_is_full(stack)) return false;
    stack->data[++stack->top] = coord;
    return true;
}

// 弹栈：取出栈顶坐标（并移除）
bool stack_pop(Stack *stack, Coord *coord) {
    if (stack_is_empty(stack)) return false;
    *coord = stack->data[stack->top--];
    return true;
}

// 取栈顶坐标（不弹出）
bool stack_peek(Stack *stack, Coord *coord) {
    if (stack_is_empty(stack)) return false;
    *coord = stack->data[stack->top];
    return true;
}

// ===================== 迷宫核心逻辑 =====================
// 测试用例指定的5x5迷宫布局
int maze[MAX_ROW][MAX_COL] = {
    {0, 1, 0, 0, 0},  // 第0行
    {0, 1, 0, 1, 0},  // 第1行
    {0, 0, 0, 0, 0},  // 第2行
    {0, 1, 1, 1, 0},  // 第3行
    {0, 0, 0, 1, 0}   // 第4行
};

// 访问标记数组：标记位置是否已遍历（避免重复/死循环）
int visited[MAX_ROW][MAX_COL];

// 方向数组：上下左右四个移动方向（行变化, 列变化）
int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// 检查坐标是否合法（在迷宫范围内 + 是通路 + 未访问）
bool is_valid(int row, int col) {
    if (row < 0 || row >= MAX_ROW || col < 0 || col >= MAX_COL) {
        return false;  // 超出迷宫边界
    }
    return maze[row][col] == 0 && visited[row][col] == 0;  // 通路且未访问
}

// DFS找路径：返回是否找到，路径存入stack
bool find_maze_path(Stack *path_stack) {
    // 初始化：清空访问标记 + 栈 + 起点入栈
    memset(visited, 0, sizeof(visited));
    stack_init(path_stack);
    Coord start = {0, 0};
    stack_push(path_stack, start);
    visited[start.row][start.col] = 1;

    // 核心DFS循环：栈非空则继续搜索
    while (!stack_is_empty(path_stack)) {
        Coord current;
        stack_peek(path_stack, &current);

        // 终止条件：到达终点(4,4)
        if (current.row == MAX_ROW - 1 && current.col == MAX_COL - 1) {
            return true;
        }

        // 遍历四个方向，寻找合法的下一步
        bool found_next = false;
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + dirs[i][0];
            int new_col = current.col + dirs[i][1];

            if (is_valid(new_row, new_col)) {
                // 找到合法方向：压栈 + 标记访问
                Coord next = {new_row, new_col};
                stack_push(path_stack, next);
                visited[new_row][new_col] = 1;
                found_next = true;
                break;  // DFS优先深入，找到一个方向就停止遍历
            }
        }

        // 回溯：四个方向都无合法路径，弹出当前坐标
        if (!found_next) {
            stack_pop(path_stack, &current);
        }
    }

    // 栈空仍未找到终点 → 无路径
    return false;
}

// 打印路径：从终点→起点（栈弹栈顺序），匹配测试用例的输出格式
void print_path(Stack *path_stack) {
    Coord coord;
    printf("路径坐标（终点→起点）：\n");
    while (stack_pop(path_stack, &coord)) {
        printf("(%d, %d)\n", coord.row, coord.col);
    }
}

// ===================== 主函数 =====================
int main() {
    Stack path_stack;

    // 打印迷宫布局（可选，测试用例不校验此部分）
    printf("5x5迷宫布局（0=通路，1=墙壁）：\n");
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // 寻找路径并输出结果
    bool has_path = find_maze_path(&path_stack);
    if (has_path) {
        print_path(&path_stack);
    } else {
        printf("No path!\n");
    }

    return 0;
}
