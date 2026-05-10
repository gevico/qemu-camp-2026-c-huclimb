#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
/*
#ifndef SIMPLE_TREE_H
#define SIMPLE_TREE_H

#include <stdbool.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct QueueNode {
    TreeNode *tree_node;
    struct QueueNode *next;
} QueueNode;

// 队列结构
typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

*/
Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    QueueNode*  node = (QueueNode* )malloc(sizeof(QueueNode));
    node->next = NULL;
    node->tree_node = tree_node;

    if (NULL == q->front && NULL == q->rear)
    {
        q->front = node;
        q->rear = node;
        return;
    }
    if (NULL != q->rear)
    {
        q->rear->next = node;
        q->rear = node;
    }
}

TreeNode* dequeue(Queue *q) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    TreeNode* result = NULL;
    if (NULL != q->front)
    {
        QueueNode*  node = q->front;
        result = q->front->tree_node;
        q->front = q->front->next;
        if(NULL == q->front)
        {
            q->rear = NULL;
        }
        free(node);
    }

    return result;
}

bool is_empty(Queue *q) {
    return (q == NULL || q->front == NULL);
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    if (size <= 0) return NULL;
    Queue* q = create_queue();

    TreeNode*  root = (TreeNode*)malloc(sizeof(TreeNode));
    root ->val = level_order[0];
    root->left = NULL;
    root->right = NULL;

    enqueue(q, root);
    int i = 1;

    while(!is_empty(q) && i < size)
    {
        TreeNode* oldnode = dequeue(q);
        if(i < size && INT_MIN != level_order[i])
        {
            TreeNode*  node = malloc(sizeof(TreeNode));
            node->val = level_order[i];
            node->left = NULL;
            node->right = NULL;

            enqueue(q,node);

            oldnode->left = node;
        }
        i++;
        if(i < size && INT_MIN != level_order[i])
        {
            TreeNode*  node = malloc(TreeNode);
            node->val = level_order[i];
            node->left = NULL;
            node->right = NULL;

            enqueue(q,node);

            oldnode->right = node;
        }
        i++;
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
}

void preorder_traversal_iterative(TreeNode *root) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
