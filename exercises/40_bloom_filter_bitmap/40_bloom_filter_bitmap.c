#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 20 位图实现 Bloom 过滤器
 * 需求：m=100 位图，k=3 个哈希函数；插入 "apple"、"banana"，查询示例
 */

typedef struct {
    unsigned char *bits;
    size_t m; /* 位图大小（比特） */
} Bloom;

static Bloom *bloom_create(size_t m) {
    // TODO: 在这里添加你的代码
    //  
    Bloom *bm = (Bloom *)malloc(sizeof(Bloom));
    if (!bm) return NULL;

    size_t bit_size = (m + 7) / 8;

    bm->bits = (unsigned char *)malloc(bit_size);
    if (!(bm->bits)) 
    {
        free(bm);
        return NULL;
    }
    
    bm->m = m;
    memset(bm->bits, 0, bit_size); 

    return bm;
}

static void bloom_free(Bloom *bf) {
    // TODO: 在这里添加你的代码
    //  
    if (!bf)
    {
        return;
    }

    free(bf->bits);
    bf->bits = NULL;
    
    free(bf);
    bf = NULL;
}

/* 位操作 */
static void set_bit(unsigned char *bm, size_t idx) {
    // TODO: 在这里添加你的代码
    //  
    size_t bm_index = idx / 8;
    size_t bm_index2 = idx %  8;
    unsigned char mask = 1 << bm_index2;

    bm[bm_index] = bm[bm_index] | mask;

}
static int test_bit(const unsigned char *bm, size_t idx) {
    // TODO: 在这里添加你的代码
    //  
    size_t bm_index = idx / 8;
    size_t bm_index2 = idx %  8;
    unsigned char mask = 1 << bm_index2;

    return 0 != (bm[bm_index] & mask);
}

/* 三个简单哈希：sum(c*k) % m */
static size_t hash_k(const char *s, size_t m, int k) {
    // TODO: 在这里添加你的代码
    //  
    size_t i = 0;
    size_t result = 0;
    while('\0' != s[i])
    {
        result += s[i] * k;  // result = (result * 31 + s[i]) * (k + 1);  // 使用质数31和k因子
        i++;
    }
    
    return result % m;
}

static void bloom_add(Bloom *bf, const char *s) {
    // TODO: 在这里添加你的代码
    //  
    size_t hash_1 = hash_k(s, bf.m, 1);
    size_t hash_2 = hash_k(s, bf.m, 2);
    size_t hash_3 = hash_k(s, bf.m, 3);

    set_bit(bf->bits, hash_1);
    set_bit(bf->bits, hash_2);
    set_bit(bf->bits, hash_3);
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    // TODO: 在这里添加你的代码
    //  
    size_t hash_1 = hash_k(s, bf->m, 1);
    size_t hash_2 = hash_k(s, bf->m, 2);
    size_t hash_3 = hash_k(s, bf->m, 3);

    int test_1 = test_bit(bf->bits, hash_1);
    int test_2 = test_bit(bf->bits, hash_2);
    int test_3 = test_bit(bf->bits, hash_3);

    return test_1 && test_2 && test_3;
}

int main(void) {
    const size_t m = 100; /* 位图大小 */
    Bloom *bf = bloom_create(m);
    if (!bf) {
        fprintf(stderr, "bloom create failed\n");
        return 1;
    }

    /* 插入元素："apple"、"banana" */
    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    /* 查询元素："apple"（应存在）、"orange"（可能存在误判） */
    int apple = bloom_maybe_contains(bf, "apple");
    int grape = bloom_maybe_contains(bf, "grape"); /* 未插入，可能误判 */

    printf("apple exists: %d\n", apple); /* 必须为 1（无假阴性） */
    printf("grape exists: %d\n", grape); /* 允许 0 或 1（可能误判） */

    bloom_free(bf);
    return 0;
}
