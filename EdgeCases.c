//
// Created by yuval on 18/05/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include "RBTree.h"

int compareInt(const void *a, const void *b)
{
    int *n1 = (int *) a;
    int *n2 = (int *) b;

    return *n1 - *n2;
}


int myForEach(const void *object, void *args){
    int cur = *(int*) object;
    return cur;
}

int main()
{
    int x =10;
    int *px = &x;

    printf("TEST: insert NULL\n");
    RBTree *tree1 = newRBTree(compareInt, free);
    printf("returned value: %d\n\n", insertToRBTree(tree1, NULL));
    freeRBTree(&tree1);

    printf("TEST: find NULL\n");
    RBTree *tree2 = newRBTree(compareInt, free);
    printf("returned value: %d\n\n", RBTreeContains(tree2, NULL));
    freeRBTree(&tree2);

    printf("TEST: delete NULL\n");
    RBTree *tree3 = newRBTree(compareInt, free);
    printf("returned value: %d\n\n", deleteFromRBTree(tree3, NULL));
    freeRBTree(&tree3);

    printf("TEST: find in empty Tree\n");
    RBTree *tree4 = newRBTree(compareInt, free);
    printf("returned value: %d\n\n", RBTreeContains(tree4, px));
    freeRBTree(&tree4);

    printf("TEST: foreach on empty Tree\n");
    RBTree *tree5 = newRBTree(compareInt, free);
    printf("returned value: %d\n\n", forEachRBTree(tree5, myForEach, px));
    freeRBTree(&tree5);

    printf("TEST: foreach with NULL as args\n");
    RBTree *tree6 = newRBTree(compareInt, free);
    printf("returned value: %d\n\n", forEachRBTree(tree6, myForEach, NULL));
    freeRBTree(&tree6);

    printf("TEST: insert on NULL tree\n");
    printf("returned value: %d\n\n", insertToRBTree(NULL, px));

    printf("TEST: delete on NULL tree\n");
    printf("returned value: %d\n\n", deleteFromRBTree(NULL, px));

    printf("TEST: search on NULL tree\n");
    printf("returned value: %d\n\n", RBTreeContains(NULL, px));

    printf("TEST: foreach on NULL tree\n");
    printf("returned value: %d\n\n", forEachRBTree(NULL, myForEach, px));

    printf("TEST DONE!\n");
}