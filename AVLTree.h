//
// Created by weikang on 2021/11/9.
//

#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H
#include <stdlib.h>
#define ALLOC(__struct) (__struct*)malloc(sizeof(__struct))
#define LH  1
#define EH  0
#define RH -1
#define bool int
#define true 1
#define false 0

struct AVLNode
{
    void* value;
    struct AVLNode* left;
    struct AVLNode* right;
    int balance;
};

struct AVLTree
{
    int count;
    bool (*compare)(void* arg1, void* arg2);
    struct AVLNode* root;
};

struct AVLNode* initAVLNode(void* v);
struct AVLTree* createAVLTree(bool (*compare)(void* arg1, void* arg2));
struct AVLTree* AVLDestroy(struct AVLTree* tree);
bool AVLInsert(struct AVLTree* tree, void * value);
bool AVLDelete(struct AVLTree* tree, void * key);
void* AVLRetrieve(struct AVLTree* tree, void * key);
void AVLTraverse(struct AVLTree* tree, void (*process)(void* value));
int AVLCount(struct AVLTree* tree);
bool AVLEmpty(struct AVLTree* tree);
bool AVLFull(struct AVLTree* tree);

static struct AVLNode* _insert(struct AVLTree* tree, struct AVLNode* node, struct AVLNode* newPtr, bool* taller);
static struct AVLNode* _delete(struct AVLTree* tree, struct AVLNode* node, void * key, bool* shorter, bool* success);
static void* _retrieve(struct AVLTree* tree, void * key, struct AVLNode* node);
static void _traversal(struct AVLNode* node, void(*process)(void* value));
static void _destroy(struct AVLNode* node);
static struct AVLNode* rotateLeft(struct AVLNode* node);
static struct AVLNode* rotateRight(struct AVLNode* node);
static struct AVLNode* insLeftBal(struct AVLNode* node, bool* taller);
static struct AVLNode* insRightBal(struct AVLNode* node, bool* taller);
static struct AVLNode* dltLeftBal(struct AVLNode* node, bool* shorter);
static struct AVLNode* dltRightBal(struct AVLNode* node, bool* shorter);

#endif //AVL_TREE_AVLTREE_H
