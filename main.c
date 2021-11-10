#include <stdio.h>
#include "AVLTree.h"

int defaultCompare(void* arg1, void* arg2)
{
    if (*(int*)(arg1) > *(int*)(arg2)) return 1;
    else if (*(int*)(arg1) < *(int*)(arg2)) return -1;
    else return 0;
}

int main() {
    struct AVLTree* tree = createAVLTree(defaultCompare);
    return 0;
}
