#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 50  // 队列最大容量（总人数）
#define COUNT 5      // 报数间隔

// 循环队列结构体定义
typedef struct {
    int data[MAX_SIZE];
    int front;       // 队头指针（出队方向）
    int rear;        // 队尾指针（入队方向）
    int size;        // 当前队列元素数量
} CircularQueue;

// 初始化循环队列
void initQueue(CircularQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

// 判断队列是否为空
bool isEmpty(CircularQueue *queue) {
    return queue->size == 0;
}

// 判断队列是否已满
bool isFull(CircularQueue *queue) {
    return queue->size == MAX_SIZE;
}

// 入队操作
bool enqueue(CircularQueue *queue, int num) {
    if (isFull(queue)) {
        return false;
    }
    queue->data[queue->rear] = num;
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    queue->size++;
    return true;
}

// 出队操作（返回队头元素，失败返回-1）
int dequeue(CircularQueue *queue) {
    if (isEmpty(queue)) {
        return -1;
    }
    int num = queue->data[queue->front];
    queue->front = (queue->front + 1) % MAX_SIZE;
    queue->size--;
    return num;
}

// 约瑟夫环核心逻辑
void josephus() {
    CircularQueue queue;
    initQueue(&queue);
    
    // 1. 初始化队列：1-50号入队
    for (int i = 1; i <= MAX_SIZE; i++) {
        enqueue(&queue, i);
    }
    
    int count = 0;  // 报数计数器
    int eliminated_count = 0;  // 淘汰人数统计
    
    // 2. 模拟报数淘汰过程（直到只剩1人）
    while (queue.size > 1) {
        int current = dequeue(&queue);
        count++;
        
        // 报数到5则淘汰，否则重新入队
        if (count % COUNT == 0) {
            eliminated_count++;
            printf("淘汰: %d\n", current);
            count = 0;  // 重置计数器
        } else {
            enqueue(&queue, current);
        }
    }
    
    // 3. 输出最后剩余的人
    int survivor = dequeue(&queue);
    printf("最后剩下的人是: %d\n", survivor);
}

int main() {
    // 执行约瑟夫环模拟
    josephus();
    return 0;
}
