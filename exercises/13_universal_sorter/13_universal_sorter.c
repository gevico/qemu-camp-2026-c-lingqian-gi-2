#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CompareFunc)(const void *, const void *);

int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compareFloat(const void *a, const void *b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int compareString(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

void sort(void *array, size_t n, size_t size, CompareFunc compare) {
    qsort(array, n, size, compare);
}

void processFile(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("错误: 无法打开文件 %s\n", filename);
        return;
    }

    int choice, n;
    if (fscanf(fin, "%d", &choice) != 1 || fscanf(fin, "%d", &n) != 1) {
        printf("错误: 文件 %s 格式不正确\n", filename);
        fclose(fin);
        return;
    }

    if (n > 20) n = 20;  // 最多支持20个元素

    printf("=== 处理数据来自: %s ===\n", filename);

    switch (choice) {
        case 1: {  // 处理整数排序
            int int_arr[20];
            // 读取整数数据
            for (int i = 0; i < n; i++) {
                fscanf(fin, "%d", &int_arr[i]);
            }
            // 调用通用排序函数
            sort(int_arr, n, sizeof(int), compareInt);
            // 输出排序结果
            printf("排序后的整数: ");
            for (int i = 0; i < n; i++) {
                printf("%d ", int_arr[i]);
            }
            printf("\n");
            break;
        }
        case 2: {  // 处理浮点数排序
            float float_arr[20];
            // 读取浮点数数据
            for (int i = 0; i < n; i++) {
                fscanf(fin, "%f", &float_arr[i]);
            }
            // 调用通用排序函数
            sort(float_arr, n, sizeof(float), compareFloat);
            // 输出排序结果（适配测试用例的小数格式）
            printf("排序后的浮点数: ");
            for (int i = 0; i < n; i++) {
                printf("%.1f ", float_arr[i]);
            }
            printf("\n");
            break;
        }
        case 3: {  // 处理字符串排序（扩展支持）
            char *str_arr[20];
            char buf[100];  // 临时缓冲区存储读取的字符串
            // 读取字符串数据并分配内存
            for (int i = 0; i < n; i++) {
                fscanf(fin, "%99s", buf);
                str_arr[i] = malloc(strlen(buf) + 1);
                if (str_arr[i] == NULL) {
                    printf("内存分配失败\n");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        free(str_arr[j]);
                    }
                    fclose(fin);
                    return;
                }
                strcpy(str_arr[i], buf);
            }
            // 调用通用排序函数
            sort(str_arr, n, sizeof(char*), compareString);
            // 输出排序结果
            printf("排序后的字符串: ");
            for (int i = 0; i < n; i++) {
                printf("%s ", str_arr[i]);
                free(str_arr[i]);  // 释放内存
            }
            printf("\n");
            break;
        }
        default:
            printf("错误: 无效的选择 %d，不支持该数据类型\n", choice);
            break;
    }

    fclose(fin);
}

int main() {
    processFile("int_sort.txt");
    processFile("float_sort.txt");
    // 可选：测试字符串排序时取消注释
    // processFile("string_sort.txt");

    return 0;
}
