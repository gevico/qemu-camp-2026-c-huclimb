#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

void initQueue(Queue* que)
{
    que->count = 0;
    que->head = 0;
    que->tail = 0;
}

// 判断队列是否为空
bool isEmpty(Queue* que) {
    return que->count == 0;
}
// 判断队列是否已满
bool isFull(Queue* que) {
    return que->count == MAX_PEOPLE;
}

// 入队操作
bool enqueue(Queue* que, People p) {
    if (isFull(que)) {
        return false;
    }
    que->data[que->tail] = p;
    que->tail = (que->tail + 1) % MAX_PEOPLE;
    que->count++;
    return true;
}

// 出队操作
People dequeue(Queue* que) {
    People p = {0};
    if (isEmpty(que)) {
        return p;
    }
    p = que->data[que->head];
    que->head = (que->head + 1) % MAX_PEOPLE;
    que->count--;
    return p;
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;

    initQueue(&q);
    // 初始化所有人（编号从1开始）
    for (int i = 1; i <= total_people; i++) {
        People p;
        p.id = i;
        enqueue(&q, p);
    }

   // 约瑟夫环：只要人数大于1，就重复淘汰过程
    while (q.count > 1) {
        // 前 report_interval - 1 人：出队后重新入队（报数安全）
        for (int i = 1; i < report_interval; i++) {
            People p = dequeue(&q);
            enqueue(&q, p);
        }
        // 第 report_interval 人：出队并淘汰（不再入队）
        dequeue(&q);
    }

    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}