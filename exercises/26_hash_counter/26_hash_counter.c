#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1024  // 哈希表大小

// 哈希表节点结构
typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;

// 哈希表结构
typedef struct {
    HashNode **table;
    int size;
} HashTable;

// djb2哈希函数
unsigned long djb2_hash(const char *str) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    unsigned long hash = 0;
    unsigned int prime = 31;
    
    while(*str)
    {
        hash += hash * prime + (unsigned char)(*str);
        str++;
    }

    return hash;
}

// 创建哈希表
HashTable *create_hash_table(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->table = calloc(size, sizeof(HashNode *));
    ht->size = size;
    return ht;
}

HashNode* createNewNode(const char *word)
{
    HashNode* node = calloc(1, sizeof(HashNode));
    if (NULL == node)
    {
        return NULL;
    }

    size_t len = strlen(word);
    node -> word = calloc(len + 1, sizeof(char));
    if (NULL == node->word)
    {
        free(node);
        return NULL;
    }
    
    strncpy(node->word, word, len);
    node->word[len]='\0';
    node->count++;

    return node;
}
// 向哈希表中插入单词
void hash_table_insert(HashTable *ht, const char *word) {
    unsigned long hash = djb2_hash(word) % ht->size;

    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    HashNode* node = ht->table[hash];

    if (NULL == node) ///< 该位置还没添加过元素
    {
        ht->table[hash] = createNewNode(word);
    }
    else
    {
        ///< 该位置添加过元素
        HashNode* cur = node;
        HashNode* pre = node;

        while(cur)
        {
            if (0 == strcmp(cur->word, word)) ///< 找到了
            {
                cur->count++;
                break;
            }

            pre = cur;
            cur = cur->next;
        }

        if (NULL == cur) ///< 遍历整个链表没找到，在链表尾部添加
        {
            pre->next = createNewNode(word);
        }

    }
}

// 从哈希表中获取所有单词及其计数
void get_all_words(HashTable *ht, HashNode **nodes, int *count) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    for (int i = 0; i < ht ->size; i++)
    {
        HashNode *tmp = ht->table[i];
        while(tmp)
        {
            nodes[*count] = tmp;
            (*count)++;
            tmp = tmp->next;
        }
    }
}

// 比较函数用于排序
int compare_nodes(const void *a, const void *b) {
    HashNode *node_a = *(HashNode **)a;
    HashNode *node_b = *(HashNode **)b;
    
    // 先按计数降序，再按字母升序
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    int result = 0;
    result = node_b->count - node_a->count;
    if (0 == result)
    {
        result = strcmp(node_a->word, node_b->word);
    }

    return result;
}

// 释放哈希表内存
void free_hash_table(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp->word);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

// 从字符串中获取下一个单词
char *get_next_word(const char **text) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    const char *ptr = *text;
    char *word = NULL;
    int len = 0;

    if (ptr == NULL || '\0' == ptr[0])
    {
        return NULL;
    }

    ///< 找到起始是字母的位置
    while((ptr[0] != '\0') && (!(('a'<=ptr[0] && ptr[0]<='z') || ('A'<=ptr[0] && ptr[0]<='Z'))))
    {
       ptr++;
    }

    if (*ptr == '\0') {
        return NULL;
    }

    const char *start = ptr;
    while(('a'<=ptr[0] && ptr[0]<='z') || ('A'<=ptr[0] && ptr[0]<='Z'))
    {
        ptr++;
    }
    len = ptr - start;
    word = calloc(len + 1, sizeof(char));
    if (NULL == word)
    {
        return NULL;
    }

    strncpy(word, start, len);
    word[len] = '\0';
    *text = ptr;
    
    return word;
}

int main(int argc, char *argv[]) {
    const char* file_path = "paper.txt";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("无法打开文件");
        return 1;
    }

    HashTable *ht = create_hash_table(TABLE_SIZE);
    char buffer[4096];
    
    printf("正在读取文件: %s\n", file_path);
    
    // 从文件读取直到EOF
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        const char *ptr = buffer;
        char *word;
        
        while ((word = get_next_word(&ptr)) != NULL) {
            hash_table_insert(ht, word);
            free(word);
        }
    }
    
    fclose(file);
    
    // 收集所有单词节点
    HashNode **nodes = malloc(TABLE_SIZE * sizeof(HashNode *));
    int node_count = 0;
    get_all_words(ht, nodes, &node_count);
    
    // 排序
    qsort(nodes, node_count, sizeof(HashNode *), compare_nodes);
    
    // 输出结果
    printf("\n单词统计结果（按频率降序排列）:\n");
    printf("%-20s %s\n", "单词", "出现次数");
    printf("-------------------- ----------\n");
    for (int i = 0; i < node_count; i++) {
        printf("%s:%d\n", nodes[i]->word, nodes[i]->count);
    }
    
    // 释放内存
    free(nodes);
    free_hash_table(ht);
    
    return 0;
}