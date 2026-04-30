#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义学生结构体
typedef struct {
    char id[10];    // 学号（足够存储S001这类格式）
    char name[20];  // 姓名
    int age;        // 年龄
} Student;

int main() {
    FILE *fp = NULL;
    Student *students = NULL;
    int count = 0;   // 学生数量
    char buffer[100];// 每行读取的缓冲区
    int i;

    // 1. 打开学生文件
    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        perror("❌ 打开文件失败");
        return 1;
    }

    // 2. 先统计学生数量（逐行读取计数）
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // 跳过空行（可选，增强健壮性）
        if (strlen(buffer) <= 1) continue;
        count++;
    }
    // 重置文件指针到开头
    fseek(fp, 0, SEEK_SET);

    // 3. 动态分配内存存储学生数组（核心：动态内存管理）
    students = (Student *)malloc(count * sizeof(Student));
    if (students == NULL) {
        perror("❌ 内存分配失败");
        fclose(fp);
        return 1;
    }

    // 4. 逐行读取并解析学生信息
    i = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(buffer) <= 1) continue; // 跳过空行
        
        // 解析格式：学号 姓名 年龄（按空格分隔）
        sscanf(buffer, "%s %s %d", students[i].id, students[i].name, &students[i].age);
        i++;
    }

    // 5. 按指定格式输出学生信息（满足测试用例检查）
    for (i = 0; i < count; i++) {
        printf("学号：%s, 姓名：%s, 年龄：%d\n", 
               students[i].id, students[i].name, students[i].age);
    }

    // 6. 资源释放（内存+文件）
    free(students);
    fclose(fp);

    return 0;
}
