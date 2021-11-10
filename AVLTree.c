//
// Created by weikang on 2021/11/9.
//
#include "AVLTree.h"

struct AVLNode* initAVLNode(void* v)
{
    struct AVLNode* n = ALLOC(struct AVLNode);
    n->value = v;
    n->left = n->right = NULL;
    n->balance = EH;

    return n;
}

struct AVLTree* createAVLTree(bool (*compare)(void* arg1, void* arg2))
{
    struct AVLTree* t = ALLOC(struct AVLTree);
    t->count = 0;
    t->compare = compare;
    t->root = NULL;

    return t;
}

bool AVLInsert(struct AVLTree* tree, void * value)
{
    bool forTaller;
    struct AVLNode* newNode = initAVLNode(value);
    tree->root = _insert(tree, tree->root, newNode, &forTaller);
    tree->count++;
    return true;
}

static struct AVLNode* _insert(struct AVLTree* tree, struct AVLNode* node, struct AVLNode* newPtr, bool* taller)
{
    if (!node)
    {
        node = newPtr;
        *taller = true;
        return node;
    }
    if (tree->compare(newPtr->value, node->value) < 0)
    {
        node->left = _insert(tree, node->left, newPtr, taller);
        if(*taller)
            switch (node->balance) {
                case LH:
                    node = insLeftBal(node, taller);
                    break;
                case EH:
                    node->balance = LH;
                    break;
                case RH:
                    node->balance = EH;
                    *taller = false;
                    break;
            } // switch
        return node;
    } // new < node
    else
    {
        // new data >= root data
        node->right = _insert(tree, node->right, newPtr, taller);
        if(*taller)
            switch (node->balance) {
                case LH:
                    node->balance = EH;
                    *taller = false;
                    break;
                case EH:
                    node->balance = RH;
                    break;
                case RH:
                    node = insRightBal(node, taller);
                    break;
            } // switch
        return node;
    } // else new data >= root data
}

static struct AVLNode* insLeftBal(struct AVLNode* node, bool* taller)
{
    struct AVLNode* rightTree;
    struct AVLNode* leftTree;

    leftTree = node->left;
    switch (leftTree->balance) {
        case LH: // Left High--Rotate Right
            node->balance = EH;
            leftTree->balance = EH;
            node = rotateRight(node);
            *taller = false;
            break;
        case EH: // This is an error
            break;
        case RH: // Right High-Requires double
                 // rotation: first left, then right
            rightTree = leftTree->right;
            switch (rightTree->balance) {
                case LH:
                    node->balance = RH;
                    leftTree->balance = EH;
                    break;
                case EH:
                case RH:
                    node->balance = EH;
                    leftTree->balance = LH;
                    break;
            }  // switch rightTree

            rightTree->balance = EH;
            // Rotate Left
            node->left = rotateLeft(leftTree);

            // Rotate Right
            node = rotateRight(node);
            *taller = false;
    } // switch
    return node;
}

static struct AVLNode* insRightBal(struct AVLNode* node, bool* taller)
{
    struct AVLNode* rightTree;
    struct AVLNode* leftTree;

    rightTree = node->right;
    switch (rightTree->balance) {
        case RH:
            node->balance = EH;
            rightTree->balance = EH;
            node = rotateLeft(node);
            break;
        case EH:
            break;
        case LH:
            leftTree = node->left;
            switch (leftTree->balance) {
                case RH:
                    node->balance = LH;
                    rightTree->balance = EH;
                    break;
                case EH:
                case LH:
                    node->balance = EH;
                    rightTree->balance = LH;
                    break;
            }

            leftTree->balance = EH;
            node->right = rotateLeft(rightTree);
            *taller = false;
    }

    return node;
}

static struct AVLNode* rotateLeft(struct AVLNode* node)
{
    struct AVLNode* temp;

    temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

static struct AVLNode* rotateRight(struct AVLNode* node)
{
    struct AVLNode* temp;

    temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

bool AVLDelete(struct AVLTree* tree, void * key)
{
    bool shorter;
    bool success;
    struct AVLNode* newNode;

    // Statements
    newNode = _delete(tree, tree->root, key, &shorter, &success);

    if (success)
    {
        tree->root = newNode;
        (tree->count)--;
        return true;
    }
    else
        return false;
}

static struct AVLNode* _delete(struct AVLTree* tree, struct AVLNode* node, void * key, bool* shorter, bool* success)
{
    // Local Definitions
    struct AVLNode* dltPtr;
    struct AVLNode* exchPtr;
    struct AVLNode* newNode;

    // Statements
    if (!node)
    {
        *shorter = false;
        *success = false;
        return NULL;
    }

    if (tree->compare(key, node->value) < 0)
    {
        node->left = _delete(tree, node->left, key, shorter, success);
        if (*shorter)
            node = dltRightBal(node, shorter);
    }  // if less
    else if (tree->compare(key, node->value) > 0)
    {
        node->right = _delete(tree, node->right, key, shorter, success);
        if (*shorter)
            node = dltLeftBal(node, shorter);
    }  // if greater
    else
    // Found equal node
    {
        dltPtr = node;
        if (!node->right)
        // Only left subtree
        {
            newNode = node->left;
            *success = true;
            *shorter = true;
            free(dltPtr);
            return newNode;
        }
        else
            if (!node->left)
                // Only right subtree
            {
                newNode = node->right;
                *success = true;
                *shorter = true;
                free(dltPtr);
                return newNode;
            }
            else
            {
                exchPtr = node->left;
                while (exchPtr->right)
                    exchPtr = exchPtr->right;
                node->value = exchPtr->value;
                node->left = _delete(tree, node->left, exchPtr->value, shorter, success);
                if (*shorter)
                    node = dltRightBal(node, shorter);
            }
    }
    return node;
}

static struct AVLNode* dltLeftBal(struct AVLNode* node, bool* shorter)
{
    // Local Definitions
    struct AVLNode* rightTree;
    struct AVLNode* leftTree;

    // Statements
    switch (node->balance) {
        case RH:
            node->balance = EH;
            break;
        case EH:
            node->balance = LH;
            *shorter = false;
            break;
        case LH:
            leftTree = node->left;
            if (leftTree->balance == RH)
                // Double rotation required
            {
                rightTree = leftTree->left;
                switch (rightTree->balance) {
                    case RH:
                        leftTree->balance = LH;
                        node->balance = EH;
                        break;
                    case EH:
                        node->balance = EH;
                        leftTree->balance = EH;
                        break;
                    case LH:
                        node->balance = RH;
                        leftTree->balance = EH;
                        break;
                }  // switch
                rightTree->balance = EH;

                // Rotate Right then Left
                node->left = rotateLeft(rightTree);
                node = rotateRight(node);
            }  // if leftTree->balance == RH
            else
            {
                // Single Rotation Only
                switch (leftTree->balance) {
                    case RH:
                    case LH:
                        node->balance = EH;
                        leftTree->balance = EH;
                        break;
                    case EH:
                        node->balance = LH;
                        leftTree->balance = RH;
                        *shorter = false;
                        break;
                }  // switch rightTree->balance
                node = rotateRight(node);
            } // else
    }
    return node;
}

static struct AVLNode* dltRightBal(struct AVLNode* node, bool* shorter)
{
    // Local Definitions
    struct AVLNode* rightTree;
    struct AVLNode* leftTree;

    // Statements
    switch (node->balance) {
        case LH:  // Deleted Left--Now balanced
            node->balance = EH;
            break;
        case EH:  // Now Right high
            node->balance = RH;
            *shorter = false;
            break;
        case RH:  // Right High - Rotate Left
            rightTree = node->right;
            if (rightTree->balance == LH)
                // Double rotation required
            {
                leftTree = rightTree->left;
                switch (leftTree->balance) {
                    case LH:
                        rightTree->balance = RH;
                        node->balance = EH;
                        break;
                    case EH:
                        node->balance = EH;
                        rightTree->balance = EH;
                        break;
                    case RH:
                        node->balance = LH;
                        rightTree->balance = EH;
                        break;
                }  // switch
                leftTree->balance = EH;

                // Rotate Right then Left
                node->right = rotateRight(rightTree);
                node = rotateLeft(node);
            }  // if rightTree->balance == LH
            else
            {
                // Single Rotation Only
                switch (rightTree->balance) {
                    case LH:
                    case RH:
                        node->balance = EH;
                        rightTree->balance = EH;
                        break;
                    case EH:
                        node->balance = RH;
                        rightTree->balance = LH;
                        *shorter = false;
                        break;
                }  // switch rightTree->balance
                node = rotateLeft(node);
            } // else
    } // switch
    return node;
}

void* AVLRetrieve(struct AVLTree* tree, void * key)
{
    // Statements
    if (!tree->root)
        return _retrieve(tree, key, tree->root);
    else
        return NULL;
}

static void* _retrieve(struct AVLTree* tree, void * key, struct AVLNode* node)
{
    // Statements
    if (node)
    {
        if (tree->compare(key, node->value) < 0)
            return _retrieve(tree, key, node->left);
        else if (tree->compare(key, node->value) > 0)
            return _retrieve(tree, key, node->right);
        else
            // Found equal key
            return node->value;
    }
    else
        // Data not in tree
        return NULL;
}

void AVLTraverse(struct AVLTree* tree, void (*process)(void* value))
{
    _traversal(tree->root, process);
}

static void _traversal(struct AVLNode* node, void(*process)(void* value))
{
    // Statements
    if(node)
    {
        _traversal(node->left, process);
        process(node->value);
        _traversal(node->right, process);
    }
}

bool AVLEmpty(struct AVLTree* tree)
{
    return (tree->count == 0);
}

bool AVLFull(struct AVLTree* tree)
{
    struct AVLNode* newNode;

    // Statements
    newNode = ALLOC(struct AVLNode);
    if (newNode)
    {
        free(newNode);
        return false;
    } else
        return true;
}

int AVLCount(struct AVLTree* tree)
{
    return tree->count;
}

struct AVLTree* AVLDestroy(struct AVLTree* tree)
{
    if (tree)
        _destroy(tree->root);
    free(tree);
    return NULL;
}

static void _destroy(struct AVLNode* node)
{
    if (node)
    {
        _destroy(node->left);
        free(node->value);
        _destroy(node->right);
        free(node);
    }
}










