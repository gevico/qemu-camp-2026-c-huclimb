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
// 交换两个学生结构体
void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

// 分区函数：选择最右边的元素作为基准，返回基准最终位置
int partition(int left, int right) {
    int pivot = students[right].score;  // 选择最后一个元素作为基准值[1](@ref)
    int i = left - 1;  // i指向小于基准的最后一个元素
    
    for (int j = left; j < right; j++) {
        if (students[j].score <= pivot) {
            i++;
            swap(&students[i], &students[j]);
        }
    }
    swap(&students[i + 1], &students[right]);  // 将基准放到正确位置
    return i + 1;  // 返回基准的最终位置
}

void quick_sort(int left, int right) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    if (left < right) {
        int pivot_index = partition(left, right);  // 一趟排序，确定基准位置
        
        quick_sort(left, pivot_index - 1);   // 递归排序左半部分
        quick_sort(pivot_index + 1, right);  // 递归排序右半部分
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