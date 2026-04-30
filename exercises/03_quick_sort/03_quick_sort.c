#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];

void quick_sort(int left, int right) {
    // 递归终止条件：左边界 >= 右边界
    if (left >= right) {
        return;
    }

    // 选择中间位置的元素作为基准值（成绩）
    int pivot = students[(left + right) / 2].score;
    int i = left, j = right;

    // 分区操作：将比基准成绩高的放左边，低的放右边
    while (i <= j) {
        // 找左侧第一个 <= 基准成绩的元素（降序，需移到右侧）
        while (students[i].score > pivot) {
            i++;
        }
        // 找右侧第一个 >= 基准成绩的元素（降序，需移到左侧）
        while (students[j].score < pivot) {
            j--;
        }

        // 交换两个元素
        if (i <= j) {
            Student temp = students[i];
            students[i] = students[j];
            students[j] = temp;
            i++;
            j--;
        }
    }

    // 递归处理左半区和右半区
    if (left < j) {
        quick_sort(left, j);
    }
    if (i < right) {
        quick_sort(i, right);
    }
}

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 03_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    // 输出结果
    printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}
