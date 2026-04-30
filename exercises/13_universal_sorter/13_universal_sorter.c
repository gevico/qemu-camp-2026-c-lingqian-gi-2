#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 通用排序函数（冒泡排序实现）
void universal_sort(void *base, size_t n, size_t size, 
                   int (*cmp)(const void *, const void *), int desc) {
    char *arr = (char *)base;
    char *temp = (char *)malloc(size);
    if (temp == NULL) {
        perror("malloc failed");
        return;
    }

    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - 1 - i; j++) {
            char *a = arr + j * size;
            char *b = arr + (j + 1) * size;
            int cmp_res = cmp(a, b);
            // 升序：a > b 则交换；降序：a < b 则交换
            if ((!desc && cmp_res > 0) || (desc && cmp_res < 0)) {
                memcpy(temp, a, size);
                memcpy(a, b, size);
                memcpy(b, temp, size);
            }
        }
    }
    free(temp);
}

// 比较函数：int类型
int cmp_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

// 比较函数：float类型
int cmp_float(const void *a, const void *b) {
    float diff = *(float *)a - *(float *)b;
    return diff > 0 ? 1 : (diff < 0 ? -1 : 0);
}

// 比较函数：字符串（按ASCII）
int cmp_str(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// 测试辅助函数：打印int数组
void print_int_arr(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 测试辅助函数：打印float数组
void print_float_arr(float *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");
}

// 测试辅助函数：打印字符串数组
void print_str_arr(char **arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%s ", arr[i]);
    }
    printf("\n");
}

// 示例主函数（测试用）
int main() {
    // 测试int排序
    int int_arr[] = {5, 2, 9, 1, 5, 6};
    size_t int_n = sizeof(int_arr) / sizeof(int);
    printf("Int原数组：");
    print_int_arr(int_arr, int_n);
    universal_sort(int_arr, int_n, sizeof(int), cmp_int, 0); // 升序
    printf("Int升序：");
    print_int_arr(int_arr, int_n);
    universal_sort(int_arr, int_n, sizeof(int), cmp_int, 1); // 降序
    printf("Int降序：");
    print_int_arr(int_arr, int_n);

    // 测试float排序
    float float_arr[] = {3.14, 1.59, 2.65, 0.78};
    size_t float_n = sizeof(float_arr) / sizeof(float);
    printf("\nFloat原数组：");
    print_float_arr(float_arr, float_n);
    universal_sort(float_arr, float_n, sizeof(float), cmp_float, 0);
    printf("Float升序：");
    print_float_arr(float_arr, float_n);

    // 测试字符串排序
    char *str_arr[] = {"apple", "banana", "cherry", "date"};
    size_t str_n = sizeof(str_arr) / sizeof(char *);
    printf("\nStr原数组：");
    print_str_arr(str_arr, str_n);
    universal_sort(str_arr, str_n, sizeof(char *), cmp_str, 0);
    printf("Str升序：");
    print_str_arr(str_arr, str_n);

    return 0;
}
