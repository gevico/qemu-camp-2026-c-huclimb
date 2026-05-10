#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */
#define PRIME (97)

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    // TODO: 在这里添加你的代码
    //   

    return key%PRIME;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    // TODO: 在这里添加你的代码
    //   
    unsigned int h = hash_int(key) % c-> bucket_count;
    HashEntry* cur = c->buckets[h];
    *pprev_next = &c->buckets[h];

    while(cur)
    {
        ///< 找到了
        if(cur->key == key)
        {
            return cur;
        }
        *pprev_next = &cur->next;
        cur = cur->next;
    }

    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    //   
    ///< Node key
    node->next = c->head->next;
    node->prev = c->head;

    if(c->head->next != NULL)
    {
        c->head->next->prev = node;
    }
    else
    {
        c->tail = node;
    }
    c->head->next = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    //   

    ///< 只做内存回收，容量由调用者维护
    int key = node->key;
    ///< 从双向链表中删除，有头节点
    node->prev->next = node->next;
    
    if(node->next != NULL)
    {
        node->next->prev = node -> prev;
    }
    else
    {
        c->tail = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;

    ///< 从hash表中删除，hash桶没有头节点，注意边界处理
    HashEntry** prev_next;
    HashEntry* cur = hash_find(c, key, &prev_next);
    if (cur != NULL)
    {
        ///< 分两种情况，找到是头节点和不是头节点
        if (*prev_next != cur)
        {
            (*prev_next)->next = cur->next;
        }
        else
        {
            *prev_next = NULL;
        }   
        free(cur);
    }
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    //   
    ///< 只把当前节点移动至头部，不做容量维护，容量由外部调用的判断
    if (c->head->next == node || (node==c->tail && node == c->head)) 
    {
        return;
    }

    node->next = c->head->next;
    node->prev = c->head;
    
    if (c->head->next != NULL) {
        c->head->next->prev = node;
    }

    c->head->next = node;
}

static LRUNode* list_pop_tail(LRUCache* c) {
    // TODO: 在这里添加你的代码
    //   

    //< 删除链表尾部的元素
    
    LRUNode *node = c->tail;
    c->tail = node->prev;
    node->prev->next = NULL;

    node->prev = NULL;
    node->next = NULL;
    
    c->size--;

    return node;
    
    ///< free(node);
    //< 从hash map中移除
    /*
    HashEntry** prev_next;
    HashEntry* cur = hash_find(c, key, &prev_next);
    if (cur != NULL)
    {
        ///< 分两种情况，找到是头节点和不是头节点
        if (*prev_next != cur)
        {
            prev_next->next = cur->next;
        }
        else
        {
            *prev_next = NULL;
        }   
        free(cur);
    }*/
    
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    // TODO: 在这里添加你的代码
    //   
    // 1、申请 LRUCache内存
    LRUCache *c = calloc(1, sizeof(LRUCache));
    if (NULL == c)
    {
        return NULL;
    }
    c->capacity = capacity;

    
    // 2、申请一个空节点作为头节点
    LRUNode *node = calloc(1, sizeof(LRUNode));
    if (NULL == node)
    {
        free(c);
        return NULL;
    }

    node->key = -1;
    
    c->head = node;
    c->tail = node;

    c->bucket_count = min(c->capacity, PRIME);
   
    //< 申请  HashEntry** buckets;的内存

    HashEntry** buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    if (NULL == buckets)
    {
        free(node);
        free(c);
        return NULL;
    }
    c->buckets = buckets;
    return c;
}

static void lru_free(LRUCache* c) {
    // TODO: 在这里添加你的代码
    //   

    LRUNode *p = c->head;
    while(p)
    {
        LRUNode* tmp = p;
        p = p->next;
        free(tmp);
    }

    for(int i = 0; i < c->bucket_count; i++)
    {
        HashEntry* bucket = c->buckets[i];
        if (NULL == bucket)
        {
            continue;
        }
        while(bucket)
        {
            HashEntry *tmp = bucket;
            bucket=bucket->next;
            free(tmp);
        }
    }

    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    // TODO: 在这里添加你的代码
    //   
        // 在哈希表中查找key
    HashEntry** prev_next;
    HashEntry* entry = hash_find(c, key, &prev_next);
    
    if (entry == NULL) {
        *out_value = -1;
        return 0; // 未找到
    }

        // 获取对应的节点
    LRUNode* node = entry->node;
    
    // 将该节点移动到链表头部（最近使用）
    list_move_to_head(c, node);
    
    // 输出值
    *out_value = node->value;
    return 1; // 找到

}

static void lru_put(LRUCache* c, int key, int value) {
    // TODO: 在这里添加你的代码
    //   
    // 检查key是否已存在
    HashEntry** prev_next;
    HashEntry* entry = hash_find(c, key, &prev_next);

    if (NULL != entry)
    {
        //< key存在
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }
    
    //< key不存在
    LRUNode* node = (LRUNode*)malloc(sizeof(LRUNode));
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    if (c->size >= c->capacity) {
        LRUNode* removed = list_pop_tail(c);
        // 从哈希表中删除被淘汰的节点
        list_remove(c, removed);
    }

    // 将新节点插入链表头部
    list_add_to_head(c, node);

    // 创建哈希表条目
    HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    new_entry->key = key;
    new_entry->node = node;
    
    // 插入到哈希桶中
    unsigned int h = hash_int(key) % c->bucket_count;
    //< 
    HashEntry* cur = c->buckets[h];
    if(cur == NULL)
    {
        c->buckets[h] = new_entry;
    }
    else
    {
        HashEntry* pre = cur;
        while(cur)
        {
            pre=cur;
            cur = cur->next;
        }
        pre->next = new_entry;
    }
    
    c->size++;
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head->next;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}

///< hash桶可以采用头插法
///< 双链表头节点拿掉