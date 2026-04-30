#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
Student temp[MAX_STUDENTS];

void merge_sort(int left, int right) {
    // 递归终止条件：子数组只有1个或0个元素时无需排序
    if (left >= right) {
        return;
    }

    // 1. 拆分：找到中间点，将数组分为左右两部分
    int mid = left + (right - left) / 2; // 避免(left+right)溢出

    // 2. 递归排序左半部分和右半部分
    merge_sort(left, mid);
    merge_sort(mid + 1, right);

    // 3. 合并：将两个有序子数组合并为一个有序数组（降序）
    int i = left;    // 左子数组起始索引
    int j = mid + 1; // 右子数组起始索引
    int k = left;    // 临时数组temp的起始索引

    // 比较左右子数组元素，按成绩降序放入临时数组
    while (i <= mid && j <= right) {
        if (students[i].score >= students[j].score) {
            temp[k++] = students[i++];
        } else {
            temp[k++] = students[j++];
        }
    }

    // 复制左子数组剩余元素到临时数组
    while (i <= mid) {
        temp[k++] = students[i++];
    }

    // 复制右子数组剩余元素到临时数组
    while (j <= right) {
        temp[k++] = students[j++];
    }

    // 将临时数组中排序好的内容复制回原数组
    for (int idx = left; idx <= right; idx++) {
        students[idx] = temp[idx];
    }
}

int main(void) {
    FILE *file = fopen("02_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 02_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d\n", n);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    merge_sort(0, n - 1);

    printf("\n归并排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}
