#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    // TODO: 在这里添加你的代码
    while(*delim != '\0')
    {
        if (c == *delim)
        {
            return 1;
        }
        delim++;
    }

    return 0;
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    // TODO: 在这里添加你的代码
    char *token_start;
    if (NULL == str)
    {
        str = *saveptr;
    }

    // 如果字符串为空，直接返回 NULL
    if (str == NULL || *str == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    // 步骤1：跳过开头的所有分隔符
    while (*str != '\0' && is_delim(*str, delim)) {
        str++;
    }

    // 如果跳过分隔符后到了字符串末尾，说明没有 token 了
    if (*str == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    token_start = str;

    while(*str != '\0' && !is_delim(*str, delim))
    {
        str++;
    }

    if(*str != '\0')
    {
        *str='\0';
        *saveptr = str + 1;
    }
    else
    {
        *saveptr = str;
    }

    return token_start;
    
}

char *strtok_r_2(char *str, const char *delim, char **saveptr)
{
    char *token_start;

    if (NULL == str)
    {
        str = *saveptr;
    }

    if (NULL == str || *str == '\0')
    {
        *saveptr = NULL;
        return NULL;
    }

    while(*str != '\0' && is_delim(*str, delim))
    {
        str++;
    }

    if (*str=='\0')
    {
        *saveptr = NULL;
        return NULL;
    }

    while(*str != '\0' && !is_delim(*str, delim))
    {
        str++;
    }

    if(*str !='\0')
    {
        *str = '\0';
        *saveptr = str +1;
    }
    else
    {
        *saveptr = str;
    }

    return token_start;
}
int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
