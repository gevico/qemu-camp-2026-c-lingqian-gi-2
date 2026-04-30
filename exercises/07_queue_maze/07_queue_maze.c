/**
 * 07_queue_maze.c
 * 迷宫求解（广度优先搜索 BFS）
 * 功能：找到从(0,0)到(4,4)的最短路径，输出路径坐标（从终点到起点）
 * 迷宫规则：0表示通路，1表示墙壁
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_ROW 5
#define MAX_COL 5
#define QUEUE_SIZE 100  // 队列最大容量

// 坐标结构体
typedef struct {
    int row;
    int col;
} Coord;

// 队列结构体
typedef struct {
    Coord data[QUEUE_SIZE];
    int front;  // 队头指针
    int rear;   // 队尾指针
} Queue;

// 初始化队列
void queue_init(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

// 判断队列是否为空
bool queue_is_empty(Queue *q) {
    return q->front == q->rear;
}

// 入队（循环队列）
bool queue_enqueue(Queue *q, Coord coord) {
    if ((q->rear + 1) % QUEUE_SIZE == q->front) {
        return false;  // 队列满
    }
    q->data[q->rear] = coord;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    return true;
}

// 出队
bool queue_dequeue(Queue *q, Coord *coord) {
    if (queue_is_empty(q)) {
        return false;
    }
    *coord = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return true;
}

int main() {
    // 定义5x5迷宫（与测试用例一致）
    int maze[MAX_ROW][MAX_COL] = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0}
    };

    // 访问标记：记录是否已访问该坐标
    int visited[MAX_ROW][MAX_COL] = {0};
    // 前驱节点：记录每个坐标的上一个坐标（用于回溯路径）
    Coord prev[MAX_ROW][MAX_COL];
    memset(prev, -1, sizeof(prev));  // 初始化为-1（无前驱）

    // 方向数组：上下左右四个方向
    Coord dirs[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    Queue q;
    queue_init(&q);

    // 起点(0,0)入队，标记为已访问
    Coord start = {0, 0};
    queue_enqueue(&q, start);
    visited[start.row][start.col] = 1;

    // 终点(4,4)
    Coord end = {4, 4};
    bool found = false;

    // BFS核心逻辑
    while (!queue_is_empty(&q) && !found) {
        Coord curr;
        queue_dequeue(&q, &curr);

        // 到达终点，终止搜索
        if (curr.row == end.row && curr.col == end.col) {
            found = true;
            break;
        }

        // 遍历四个方向
        for (int i = 0; i < 4; i++) {
            int new_row = curr.row + dirs[i].row;
            int new_col = curr.col + dirs[i].col;

            // 检查坐标合法性：在范围内、不是墙壁、未访问
            if (new_row >= 0 && new_row < MAX_ROW &&
                new_col >= 0 && new_col < MAX_COL &&
                maze[new_row][new_col] == 0 &&
                !visited[new_row][new_col]) {
                
                Coord next = {new_row, new_col};
                queue_enqueue(&q, next);
                visited[new_row][new_col] = 1;
                // 记录前驱节点
                prev[new_row][new_col] = curr;
            }
        }
    }

    // 处理结果
    if (!found) {
        printf("No path!\n");
        return 0;
    }

    // 回溯路径：从终点到起点
    Coord path[QUEUE_SIZE];
    int path_len = 0;
    Coord curr = end;

    while (curr.row != -1 && curr.col != -1) {
        path[path_len++] = curr;
        curr = prev[curr.row][curr.col];
    }

    // 输出路径坐标（每行一个，格式：(行, 列)）
    for (int i = 0; i < path_len; i++) {
        printf("(%d, %d)\n", path[i].row, path[i].col);
    }

    return 0;
}
