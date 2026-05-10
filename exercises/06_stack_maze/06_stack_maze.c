#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_STACK_SIZE 100

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

// 访问标记数组，记录哪些位置已经被访问过
int visited[MAX_ROW][MAX_COL] = {0};

// 四个方向：上、右、下、左（顺时针）
int directions[2] = {
    {-1, 0},  // 上
    {0, 1},   // 右
    {1, 0},   // 下
    {0, -1}   // 左
};

// 位置结构体
typedef struct {
    int x;
    int y;
} Position;

// 栈元素结构体
typedef struct {
    Position pos;
    int direction;  // 下一个要尝试的方向
} StackElement;

// 栈结构体
typedef struct {
    StackElement data[MAX_STACK_SIZE];
    int top;
} Stack;

// 初始化栈
void initStack(Stack *s) {
    s->top = -1;
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            visited[i][j] = 0;
        }
    }
}

// 判断栈是否为空
bool isEmpty(Stack *s) {
    return s->top == -1;
}

// 判断栈是否已满
bool isFull(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

// 入栈
bool push(Stack *s, Position pos, int direction) {
    if (isFull(s)) {
        printf("栈已满！\n");
        return false;
    }
    s->top++;
    s->data[s->top].pos = pos;
    s->data[s->top].direction = direction;
    return true;
}

// 出栈
bool pop(Stack *s, Position *pos, int *direction) {
    if (isEmpty(s)) {
        return false;
    }
    *pos = s->data[s->top].pos;
    *direction = s->data[s->top].direction;
    s->top--;
    return true;
}

// 获取栈顶元素
bool peek(Stack *s, Position *pos, int *direction) {
    if (isEmpty(s)) {
        return false;
    }
    *pos = s->data[s->top].pos;
    *direction = s->data[s->top].direction;
    return true;
}

// 判断位置是否在迷宫范围内且可通行
bool isValid(int x, int y) {
    return (x >= 0 && x < MAX_ROW && y >= 0 && y < MAX_COL 
            && maze[x][y] == 0 && !visited[x][y]);
}

// 输出路径
void printPath(Stack *s) {
    if (isEmpty(s)) {
        printf("路径为空！\n");
        return;
    }
    
    printf("找到的路径（从起点到终点）：\n");
    
    // 将栈中元素按顺序输出
    // 由于栈是后进先出，我们需要先倒序输出
    StackElement path[MAX_STACK_SIZE];
    int pathLen = 0;
    
    // 将栈中元素复制到临时数组
    for (int i = 0; i <= s->top; i++) {
        path[pathLen++] = s->data[i];
    }
    
    // 从起点到终点输出
    for (int i = 0; i < pathLen; i++) {
        printf("(%d, %d)", path[i].pos.x, path[i].pos.y);
        if (i < pathLen - 1) {
            printf(" → ");
        }
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n\n路径长度：%d步\n", pathLen - 1);
    
    // 可视化路径
    printf("\n路径可视化（*表示路径）：\n");
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            bool onPath = false;
            for (int k = 0; k < pathLen; k++) {
                if (path[k].pos.x == i && path[k].pos.y == j) {
                    onPath = true;
                    break;
                }
            }
            if (maze[i][j] == 1) {
                printf("■ ");
            } else if (onPath) {
                printf("* ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

// 迷宫求解主函数
bool solveMaze(Position start, Position end) {
    Stack s;
    initStack(&s);
    
    // 起点入栈，方向初始化为-1（表示尚未探索）
    push(&s, start, -1);
    visited[start.x][start.y] = 1;
    
    while (!isEmpty(&s)) {
        Position currentPos;
        int currentDir;
        
        // 获取栈顶元素
        peek(&s, &currentPos, &currentDir);
        
        // 检查是否到达终点
        if (currentPos.x == end.x && currentPos.y == end.y) {
            printPath(&s);
            return true;
        }
        
        // 从当前方向+1开始尝试下一个方向
        bool found = false;
        int nextDir = currentDir + 1;
        
        while (nextDir < 4 && !found) {
            int nextX = currentPos.x + directions[nextDir][0];
            int nextY = currentPos.y + directions[nextDir][1];
            
            if (isValid(nextX, nextY)) {
                // 找到可通行的方向，更新栈顶元素的方向
                s.data[s.top].direction = nextDir;
                
                // 将新位置入栈
                Position newPos = {nextX, nextY};
                push(&s, newPos, -1);
                visited[nextX][nextY] = 1;
                
                found = true;
            } else {
                nextDir++;
            }
        }
        
        // 如果所有方向都不可通行，则回溯
        if (!found) {
            pop(&s, &currentPos, &currentDir);
            // 注意：这里不需要恢复visited状态，因为回溯到前一个节点后
            // 前一个节点会尝试其他方向，不会再次走回这个死胡同
            // 但为了严谨性，可以在pop时恢复，不过会导致更多的尝试
            // visited[currentPos.x][currentPos.y] = 0;
        }
    }
    
    printf("未找到路径！\n");
    return false;
}

int main(void) {
    Position start = {0, 0};  // 起点
    Position end = {4, 4};    // 终点
    
    printf("迷宫求解器\n");
    printf("==========\n\n");
    
    printf("迷宫地图（0:通道，1:障碍物）：\n");
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n起点：(%d, %d)\n", start.x, start.y);
    printf("终点：(%d, %d)\n\n", end.x, end.y);
    
    solveMaze(start, end);
    
    return 0;
}