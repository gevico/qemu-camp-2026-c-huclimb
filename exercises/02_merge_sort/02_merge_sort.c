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

// 合并两个已排序的子数组 [left, mid] 和 [mid+1, right]
void merge(int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;  // 左子数组大小
    int n2 = right - mid;     // 右子数组大小
    
    // 将数据拷贝到临时数组
    for (i = 0; i < n1; i++)
        temp[i] = students[left + i];
    for (j = 0; j < n2; j++)
        temp[n1 + j] = students[mid + 1 + j];
    
    // 合并临时数组到原数组
    i = 0;      // 左子数组索引
    j = n1;     // 右子数组索引（在temp中的起始位置）
    k = left;   // 原数组索引
    
    while (i < n1 && j < n1 + n2) {
        if (temp[i].score <= temp[j].score) {
            students[k] = temp[i];
            i++;
        } else {
            students[k] = temp[j];
            j++;
        }
        k++;
    }
    
    // 拷贝左子数组剩余元素
    while (i < n1) {
        students[k] = temp[i];
        i++;
        k++;
    }
    
    // 拷贝右子数组剩余元素
    while (j < n1 + n2) {
        students[k] = temp[j];
        j++;
        k++;
    }
}

void merge_sort(int left, int right) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    if (left < right) {
        int mid = left + (right - left) / 2;  // 防止整数溢出
        
        // 递归排序左半部分
        merge_sort(left, mid);
        // 递归排序右半部分
        merge_sort(mid + 1, right);
        // 合并已排序的子数组
        merge(left, mid, right);
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