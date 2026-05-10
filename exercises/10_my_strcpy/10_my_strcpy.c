#include <stdio.h>

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    
	// TODO: 在这里添加你的代码
    // I AM NOT DONE
    char *tmp = source;
    char *dst = destination;
    int max_len = sizeof(destination) - 1;  // 预留结束符位置
    int copied = 0;
    while(*tmp && copied < max_len)
    {
        *dst = *tmp;
        tmp++;
        dst++;
    }
    *dst='\0';
    
    printf("拷贝后的字符串: %s", destination);
    
    return 0;
}