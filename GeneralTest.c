//
// Created by yuval on 18/05/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include "RBTree.h"

/* Compute the "height" of a tree -- the number of
    nodes along the longest path from the root node
    down to the farthest leaf node.*/
int height(struct Node *node)
{
    if (node == NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int a = *(int *) node->data;
        int b = *(int *) node->data;

        int lheight = height(node->left);
        int rheight = height(node->right);

        /* use the larger one */
        if (lheight > rheight)
            return (lheight + 1);
        else return (rheight + 1);
    }
}

/* Print nodes at a given level */
void printGivenLevel(struct Node *root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        printf("(%d, %d)", (*(int *) root->data), root->color);
    else if (level > 1)
    {
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}


/* Function to print level order traversal a tree*/
void printLevelOrder(struct Node *root)
{
    int h = height(root);
    int i;
    for (i = 1; i <= h; i++)
    {
        printGivenLevel(root, i);
        printf("\n");
    }
}


int compareInt(const void *a, const void *b)
{
    int *n1 = (int *) a;
    int *n2 = (int *) b;
    int num1 = *n1;
    int num2 = *n2;

    return num1 - num2;
}


void insert(RBTree *tree, int *n)
{
    printf("Insert- %d\n", *n);
    int result = insertToRBTree(tree, n);
    printf("returned value- %d\n", result);
    printf("Tree-\n");
    printLevelOrder(tree->root);
    printf("\n");
}

void myDelete(RBTree *tree, int *n)
{
    printf("delete %d-\n", *n);
    int result = deleteFromRBTree(tree, n);
    printf("returned value- %d\n", result);
    printf("Tree-\n");
    printLevelOrder(tree->root);
    printf("\n");
}

int myForEach(const void *object, void *args)
{
    int curNum = *(int *) object;
    int arg = *(int *) args;
    if (curNum >= arg)
    {
        return 0;
    }
    return 1;
}

int main()
{
    int *pNums[10];

    for (int j = 0; j < 10; ++j)
    {
        pNums[j] = (int *) malloc(sizeof(int));
        *(pNums[j]) = j;
    }

    RBTree *tree = newRBTree(compareInt, free);

    printf("---TEST INSERT---\n");
    //----Insert
    insert(tree, pNums[5]);
    insert(tree, pNums[7]);
    insert(tree, pNums[3]);
    insert(tree, pNums[2]);
    insert(tree, pNums[6]);
    insert(tree, pNums[8]);
    insert(tree, pNums[9]);
    insert(tree, pNums[1]);
    insert(tree, pNums[0]);
    insert(tree, pNums[4]);
    printf("\n---TEST DUPLICATE INSERT---\n");
    //Duplicate insert:
    insert(tree, pNums[0]);
    insert(tree, pNums[2]);
    insert(tree, pNums[4]);

    // test delete not in tree value
    printf("--- TEST DELETE NON EXIST VALUE--\n");
    int lala = 11;
    int *plala = &lala;
    myDelete(tree, plala);

    printf("\n---TEST DELETE---\n");
    //--Delete
    myDelete(tree, pNums[5]);
    myDelete(tree, pNums[1]);
    myDelete(tree, pNums[4]);
    myDelete(tree, pNums[8]);
    myDelete(tree, pNums[0]);
    myDelete(tree, pNums[2]);
    myDelete(tree, pNums[9]);
    myDelete(tree, pNums[6]);
    myDelete(tree, pNums[7]);
    myDelete(tree, pNums[3]);



    for (int j = 0; j < 10; ++j)
    {
        pNums[j] = (int *) malloc(sizeof(int));
        *(pNums[j]) = j;
    }


    // insert again
    insert(tree, pNums[1]);
    insert(tree, pNums[2]);
    insert(tree, pNums[3]);
    insert(tree, pNums[4]);
    insert(tree, pNums[5]);
    insert(tree, pNums[8]);
    insert(tree, pNums[9]);
    insert(tree, pNums[7]);
    insert(tree, pNums[6]);
    insert(tree, pNums[0]);


    printf("\n---TEST FOREACH---\n");
    int maxAllowed = 10;
    int *pMaxAllowed = &maxAllowed;
    printf("Test success in for each- returned value: %d\n", forEachRBTree(tree, myForEach, pMaxAllowed));
    maxAllowed = 8;
    printf("Test failure in for each- returned value: %d\n", forEachRBTree(tree, myForEach, pMaxAllowed));

    printf("\n---TEST RBTreeContains---\n");
    printf("Test exist element: %d\n", RBTreeContains(tree, pNums[0]));

    printf("Test non exist element: %d\n", RBTreeContains(tree, plala));


    printf("\n---TEST FREE_TREE---\n");
    freeRBTree(&tree);


    printf("TEST DONE!\n");
}