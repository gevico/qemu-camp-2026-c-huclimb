#include <stdio.h>

int countWord(char *str)
{
    char *ptr = str;
    int count = 0;

    ///< 跳过非字母
    while(*ptr)
    {
        while((ptr[0] != '\0') && (!(('a'<=ptr[0] && ptr[0]<='z') || ('A'<=ptr[0] && ptr[0]<='Z'))))
        {
            ptr++;
        }

        if('\0' == ptr[0])
        {
            break;
        }

        while (('a'<=ptr[0] && ptr[0]<='z') || ('A'<=ptr[0] && ptr[0]<='Z') || ('\'' == ptr[0]))
        {
            ptr++;
        }

        count++;
    }

    return count;

}

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    // TODO: 在这里添加你的代码

    wordCount = countWord(str);
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}