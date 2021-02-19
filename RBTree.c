/** @file RBTree.c
* @author  Yair Escott <yair.95@gmail.com>
*
* @brief implementing a red black tree
*/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>

// -------------------------- const definitions -------------------------

// uses recursion to add a new node to the tree
int treeRBinsert(RBTree* tree, Node* root, Node* new);

//constructs a new RBTree with the given CompareFunc.
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc);

// add an item to the tree
int insertToRBTree(RBTree *tree, void *data);

// fixes the tree's colors after a new node was added
void fixTreeInsert(RBTree* tree, Node* newlyAdded);

// returns the nodes parent
Node * getParent(Node* node);

// Activate a function on each item of the tree. the order is an ascending order. if one of the
// activations of the function returns 0, the process stops.
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args);

// returns the nodes uncle
Node * getUncle(RBTree *tree, Node* node);

// rotates left
void rotateLeft(RBTree *tree, Node* node);

// rotates right
void rotateRight(RBTree *tree, Node* node);

// remove an item from the tree
int deleteFromRBTree(RBTree *tree, void *data);

// check whether the tree RBTreeContains this item.
int RBTreeContains(const RBTree *tree, const void *data);

// free all memory of the data structure
void freeRBTree(RBTree **tree);

// finds the successor of a node
Node * inOrderSuccessor(Node *n);

// returns the minimum node
Node * minValue(Node* node);

// helper function for RBTreeContains
int contains(const RBTree *tree, Node* node, const void * data);

// finds a node in the tree
Node* nodeFinder(RBTree * tree, Node * node, void* data);

// fixes the tree's colors after a node was deleted
void fixTreeDelete(RBTree* tree, Node* node);

// the first case- if the node is red
void delete1(RBTree* tree, Node* node);

// the second case- if the node is black and its kid is red
void delete2(RBTree* tree, Node* node, char side);

// the third case- if the node is black and its kid is black or NULL
void delete3(RBTree* tree, Node* node, char side);

// replaces one node with another
void treeReplaceNode(Node* n, Node* child);

// step 2 of the left rotate
void rotateLeft2(RBTree *tree, Node* node);

// step 2 of the right rotate
void rotateRight2(RBTree *tree, Node* node);

// if the S is black and both of its kids are black
void deleteB(RBTree* tree, Node* parent, char side);

// if the S is RED
void deleteC(RBTree* tree, Node* parent, char side);

// if the S is Black and the far kid from C is black and the closer one is red
void deleteD(RBTree* tree, Node* parent, char side);

// if the S is Black and the far kid from C is RED
void deleteE(RBTree* tree, Node* parent, char side);

// removes the node
char removeM(RBTree* tree, Node* node);

// rotates left
void rotateLeftDelete(RBTree *tree, Node *parent);

// rotates right
void rotateRightDelete(RBTree *tree, Node *parent);

// helps free the tree
void freeHelper(Node* node, FreeFunc func);

// helps go over the nodes
int forEachHelper(Node *node, forEachFunc func, void* args);

// ------------------------------ functions -----------------------------

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    struct RBTree * tree;
    tree = (struct RBTree *)malloc(sizeof(struct RBTree));
    if(tree == NULL)
    {
        return NULL;
    }
    tree->root = NULL;
    tree->compFunc = compFunc;
    tree->freeFunc = freeFunc;
    tree->size = 0;
    return tree;
}

/**
 * @brief uses recursion to add a new node to the tree
 * @param tree the RB tree
 * @param root the recursed node
 * @param new the node to add
 * @return 0 if failed 1 if not
 */
int treeRBinsert(RBTree* tree, Node* root, Node* new)
{
    int comp = tree->compFunc(root->data, new->data);
    if (comp == 0)
    {
        //duplicate
        return 0;
    }
    else if (comp < 0)
    {
        if (root->right != NULL)
        {
            treeRBinsert(tree, root->right, new);
        }
        else
        {
            root->right = new;
            new->parent = root;
        }
    }
    else
    {
        if (root->left != NULL)
        {
            treeRBinsert(tree, root->left, new);
        }
        else
        {
            root->left = new;
            new->parent = root;
        }
    }
    return 1;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if(tree == NULL)
    {
        return 0;
    }
    if(data == NULL)
    {
        return 0;
    }
    if(RBTreeContains(tree, data) == 1)
    {
        return 0;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL)
    {
        return 0;
    }
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->color = RED;
    newNode->data = data;
    if(tree->root == NULL)
    {
        newNode->color = BLACK;
        tree->root = newNode;
        tree->size += 1;
        return 1;
    }
    else if(treeRBinsert(tree, tree->root, newNode) == 0)
    {
        return 0;
    }
    else
    {
        fixTreeInsert(tree, newNode);
        tree->size += 1;
        return 1;
    }
}

/**
 * @brief returns the nodes parent
 * @param node the node
 * @return  the parent
 */
Node * getParent(Node* node)
{
    if(node == NULL || node->parent == NULL)
    {
        return NULL;
    }
    else
    {
        return node->parent;
    }
}

/**
 * @brief returns the nodes uncle
 * @param node the node
 * @return  the uncle
 */
Node * getUncle(RBTree *tree, Node* node)
{
    Node * parent = getParent(node);
    Node* grandParent = getParent(parent);
    if(grandParent != NULL)
    {
        if(grandParent->left != NULL)
        {
            if (tree->compFunc(grandParent->left->data, parent->data) != 0)
            {
                return grandParent->left;
            }
            if(grandParent->right != NULL)
            {
                return grandParent->right;
            }
        }
    }
    return NULL;
}

/**
 * @brief fixes the tree's colors after a new node was added
 * @param tree the RB tree
 * @param newlyAdded the new node
 */
void fixTreeInsert(RBTree* tree, Node* newlyAdded)
{
    Node* uncle = getUncle(tree, newlyAdded);
    Node* parent = getParent(newlyAdded);
    Node* grandParent = getParent(parent);
    // case 1
    if (tree->compFunc(tree->root->data, newlyAdded->data) == 0)
    {
        newlyAdded->color = BLACK;
    }
    else
    {
        //case 2- no need to change
        if (parent->color == RED)
        {
            //case 3
            // if the node has an uncle then it has a grandparent
            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                fixTreeInsert(tree, grandParent);
            }
            // if the uncle is black or null
            else
            {
                if (parent->left != NULL && tree->compFunc(newlyAdded->data, parent->left->data)
                    == 0)
                {
                    rotateRight(tree, newlyAdded);
                }
                else
                {
                    rotateLeft(tree, newlyAdded);
                }
            }
        }
    }
}

/**
 * @brief rotates left
 * @param node the node to rotate
 */
void rotateLeft(RBTree *tree, Node* node)
{
    Node* parent = getParent(node);
    Node* grandParent = getParent(getParent(node));
    //case the right kid of a left kid
    if(grandParent->left != NULL && tree->compFunc(parent->data, grandParent->left->data) == 0)
    {
        grandParent->left = node;
        if(node->left != NULL)
        {
            node->left->parent = parent;
        }
        parent->right = node->left;
        node->left = parent;
        node->parent = grandParent;
        parent->parent = node;
        rotateRight2(tree, parent);
        node->color = BLACK;
    }
    // case right kid of  right kid
    else
    {
        rotateLeft2(tree, node);
        parent->color = BLACK;
    }
    grandParent->color = RED;
}

/**
 * @brief rotates right
 * @param node the node to rotate
 */
void rotateRight(RBTree *tree, Node* node)
{
    Node* parent = getParent(node);
    Node* grandParent = getParent(getParent(node));
    // case left kid of a right kid
    if(grandParent->right != NULL && tree->compFunc(parent->data, grandParent->right->data) == 0)
    {
        grandParent->right = node;
        if(node->right != NULL)
        {
            node->right->parent = parent;
        }
        parent->left = node->right;
        node->right = parent;
        node->parent = grandParent;
        parent->parent = node;
        rotateLeft2(tree, parent);
        node->color = BLACK;

    }
        //case left kid of a left kid
    else if(tree->compFunc(parent->data, grandParent->left->data) == 0)
    {
        rotateRight2(tree, node);
        parent->color = BLACK;
    }
    grandParent->color = RED;
}


/**
 * @brief step 2 of the left rotate
 * @param tree the tree
 * @param node the new node
 */
void rotateLeft2(RBTree *tree, Node* node)
{
    Node * grandParent = getParent(getParent(node));
    Node* parent = getParent(node);

    grandParent->right = parent->left;
    if(parent->left != NULL)
    {
        parent->left->parent = grandParent;
    }
    parent->left = grandParent;
    // grandparent is the root
    if(grandParent->parent == NULL)
    {
        tree->root = parent;
        parent->parent = NULL;
        parent->color = BLACK;
        grandParent->parent = parent;
    }
    else
    {
        if(tree->compFunc(grandParent->parent->left->data, grandParent->data) == 0)
        {
            grandParent->parent->left = parent;
        }
        else
        {
            grandParent->parent->right = parent;
        }
        parent->parent = grandParent->parent;
        grandParent->parent = parent;
    }
}

/**
 * @brief step 2 of the right rotate
 * @param tree the tree
 * @param node the new node
 */
void rotateRight2(RBTree *tree, Node* node)
{
    Node * grandParent = getParent(getParent(node));
    Node* parent = getParent(node);
    grandParent->left = parent->right;
    if(parent->right != NULL)
    {
        parent->right->parent = grandParent;
    }
    parent->right = grandParent;
    // grandparent is the root
    if(grandParent->parent == NULL)
    {
        tree->root = parent;
        parent->parent = NULL;
        parent->color = BLACK;
        grandParent->parent = parent;
    }
    else
    {
        if(tree->compFunc(grandParent->parent->left->data, grandParent->data) == 0)
        {
            grandParent->parent->left = parent;
        }
        else
        {
            grandParent->parent->right = parent;
        }
        parent->parent = grandParent->parent;
        grandParent->parent = parent;
    }
    grandParent->color = RED;
}

/**
 * @brief finds the successor of a node
 * @param root the root of the tree
 * @param n the node
 * @return the successor
 */
Node * inOrderSuccessor(Node *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    if (n->right != NULL)
    {
        return minValue(n->right);
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief returns the minimum node
 * @param node the node to search in its subtree
 * @return the minimal node
 */
Node * minValue(Node* node)
{
    Node* current = node;
    while (current->left != NULL)
    {
        current = current->left;

        if(current == NULL)
        {
            return NULL;
        }
    }
    return current;
}


/**
 * @brief replaces one node with another
 * @param n the node to replace
 * @param child the node to replace n with
 */
void treeReplaceNode(Node* n, Node* child)
{
    void *temp = n->data;
    n->data = child->data;
    child->data = temp;
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    if(tree == NULL)
    {
        return 0;
    }
    if(data == NULL)
    {
        return 0;
    }
    if(RBTreeContains(tree, data) == 0)
    {
        return 0;
    }
    Node* node = nodeFinder(tree, tree->root, data);
    if(node == NULL)
    {
        return 0;
    }
    Node* suc = inOrderSuccessor(node);
    if(suc != NULL)
    {
        treeReplaceNode(node, suc);
        fixTreeDelete(tree, suc);
    }

    else
    {
        fixTreeDelete(tree, node);
    }
    tree->size -= 1;
    return 1;
}

/**
 * @brief fixes the tree's colors after a node was deleted
 * @param tree the RB tree
 * @param node the new node
 */
void fixTreeDelete(RBTree* tree, Node* node)
{
    if(node->color == RED)
    {
        delete1(tree, node);
    }
    else if(node->color == BLACK)
    {
        if(node->right != NULL && node->right->color == RED)
        {
            delete2(tree, node, 'r');
        }
        else if(node->left != NULL && node->left->color == RED)
        {
            delete2(tree, node, 'l');

        }
        else
        {
            Node* parent = node->parent;
            char side = removeM(tree, node);
            if(side != 't')
            {
                delete3(tree, parent, side);
            }
        }
    }
}

/**
 * @brief if the S is black and both of its kids are black
 * @param parent the parent of the node deleted
 * @param tree the tree
 */
void deleteB(RBTree* tree, Node* parent, char side)
{
    // case i
    if(parent->color == RED)
    {
        parent->color = BLACK;
        if(side == 'l')
        {
            parent->right->color = RED;
        }
        else if(side == 'r')
        {
            parent->left->color = RED;
        }
    }
    else
    {
        if(side == 'l')
        {
            parent->right->color = RED;
        }
        else
        {
            parent->left->color = RED;
            if(parent->parent != NULL)
            {
                if (tree->compFunc(parent->data, parent->parent->left->data) == 0)
                {
                    delete3(tree, parent->parent, 'l');
                }
                else
                {
                    delete3(tree, parent->parent, 'r');
                }
            }
        }
    }
}

/**
 * @brief if the S is Black and the far kid from C is black and the closer one is red
 * @param parent the parent of the node deleted
 * @param tree the tree
 */
void deleteD(RBTree* tree, Node* parent, char side)
{
    if(side == 'l')
    {
        parent->right->left->color = BLACK;
        parent->right->color = RED;
        rotateRightDelete(tree, parent->right);
        deleteE(tree, parent, side);
    }
    else
    {
        parent->left->right->color = BLACK;
        parent->left->color = RED;
        rotateLeftDelete(tree, parent->left);
        deleteE(tree, parent, side);
    }
}

/**
 * @brief if the S is Black and the far kid from C is RED
 * @param parent the parent of the node deleted
 * @param tree the tree
 */
void deleteE(RBTree* tree, Node* parent, char side)
{
    if(side == 'l')
    {
        Node* S = parent->right;
        Color par = parent->color;
        Color Sc = parent->right->color;
        parent->color = Sc;
        parent->right->color = par;
        rotateLeftDelete(tree, parent);
        S->right->color = BLACK;
    }
    else if(side == 'r')
    {
        Node* S = parent->left;
        Color par = parent->color;
        Color Sc = parent->left->color;
        parent->color = Sc;
        parent->left->color = par;
        rotateRightDelete(tree, parent);
        S->left->color = BLACK;
    }

}

/**
 * @brief if the S is RED
 * @param parent the parent of the node deleted
 * @param tree the tree
 */
void deleteC(RBTree* tree, Node* parent, char side)
{
    if(side == 'l')
    {
        Color par = parent->color;
        Color  S = parent->right->color;
        parent->color = S;
        parent->right->color = par;
        // rotate left
        rotateLeftDelete(tree, parent);
    }
    else if(side == 'r')
    {
        Color par = parent->color;
        Color  S = parent->left->color;
        parent->color = S;
        parent->left->color = par;
        // rotate right
        rotateRightDelete(tree, parent);
    }
    delete3(tree, parent, side);
}

/**
 * @brief rotates right
 * @param tree the tree
 * @param parent the P node
 */
void rotateRightDelete(RBTree *tree, Node *parent)
{
    Node* s = parent->left;
    parent->left = s->right;
    if(s-> right != NULL)
    {
        s->right->parent = parent;
    }
    s->right = parent;
    s->parent = parent->parent;
    if(parent->parent != NULL)
    {
        if(parent->parent->left != NULL && tree->compFunc(parent->data, parent->parent->left->data)
           == 0)
        {
            parent->parent->left = s;
        }
        else
        {
            parent->parent->right = s;
        }
    }
    else
    {
        tree->root = s;
    }
    parent-> parent = s;
}

/**
 * @brief rotates left
 * @param tree the tree
 * @param parent the P node
 */
void rotateLeftDelete(RBTree *tree, Node *parent)
{
    Node* s = parent->right;
    parent->right = s->left;
    if(s-> left != NULL)
    {
        s->left->parent = parent;
    }
    s->left = parent;
    s->parent = parent->parent;
    if(parent->parent != NULL)
    {
        if(tree->compFunc(parent->data, parent->parent->left->data) == 0)
        {
            parent->parent->left = s;
        }
        else
        {
            parent->parent->right = s;
        }
    }
    else
    {
        tree->root = s;
    }
    parent-> parent = s;
}

/**
 * @brief the third case- if the node is black and its kid is black - both of its kis are null
 * @param tree the treeRBTree* tree, Node* node
 * @param node the node to delete
 */
void delete3(RBTree* tree, Node* pnode, char side)
{
    if (side == 'l')
    {
        if(!(pnode->left != NULL && tree->compFunc(tree->root->data, pnode->left->data) != 0))
        {
            if (pnode->right != NULL && pnode->right->color == BLACK)
            {
                if ((pnode->right->right == NULL || pnode->right->right->color == BLACK) &&
                    (pnode->right->left == NULL || pnode->right->left->color == BLACK))
                {
                    deleteB(tree, pnode, side);
                }
                else if (pnode->right->left != NULL && pnode->right->left->color == RED &&
                         (pnode->right->right == NULL || pnode->right->right->color == BLACK))
                {
                    deleteD(tree, pnode, side);
                }
                else if (pnode->right->right != NULL && pnode->right->right->color == RED)
                {
                    deleteE(tree, pnode, side);
                }
            }
            else
            {
                deleteC(tree, pnode, side);
            }
        }
    }
    else if (side == 'r')
    {
        if(!(pnode->right != NULL && tree->compFunc(tree->root->data, pnode->right->data) == 0))
        {
            if (pnode->left != NULL && pnode->left->color == BLACK)
            {
                if ((pnode->left->right == NULL || pnode->left->right->color == BLACK) &&
                    (pnode->left->left == NULL || pnode->left->left->color == BLACK))
                {
                    deleteB(tree, pnode, side);
                }
                else if (pnode->left->right != NULL && pnode->left->right->color == RED &&
                         (pnode->left->left == NULL || pnode->left->left->color == BLACK))
                {
                    deleteD(tree, pnode, side);
                }
                else if (pnode->left->left != NULL && pnode->left->left->color == RED)
                {
                    deleteE(tree, pnode, side);
                }
            }
            else
            {
                deleteC(tree, pnode, side);
            }
        }
    }
}

/**
 * @brief removes the node
 * @param tree the tree
 * @param node the node to delete
 */
char removeM(RBTree* tree, Node* node)
{
    if(node->parent != NULL)
    {
        if (node->parent->left != NULL && tree->compFunc(node->data, node->parent->left->data) == 0)
        {
            node->parent->left = node->left;
            if (node->left != NULL)
            {
                node->left->parent = node->parent;
            }
            tree->freeFunc(node->data);
            free(node);
            node = NULL;
            return 'l';
        }
        else
        {
            node->parent->right = node->right;
            if (node->right != NULL)
            {
                node->right->parent = node->parent;
            }
            tree->freeFunc(node->data);
            free(node);
            node = NULL;
            return 'r';
        }
    }
    else
    {
        // case A the node is the root
        tree->root = node->right;
        if (node->right != NULL)
        {
            node->right->parent = NULL;
        }
        tree->freeFunc(node->data);
        free(node);
        node = NULL;
        return 't';
    }
}

/**
 * @brief the second case- if the node is black and its kid is red
 * @param tree the tree
 * @param node the node to delete
 */
void delete2(RBTree* tree, Node* node, char side)
{
    if(side == 'r')
    {
        node->right->parent = node->parent;
        if(tree->compFunc(node->parent->right->data, node->data) == 0)
        {
            node->parent->right = node->right;
        }
        else
        {
            node->parent->left = node->right;
        }
        node->right->color = BLACK;
    }
    else
    {
        if(node->parent == NULL)
        {
            tree->root = node->left;
            node->left->parent = node->parent;
        }
        else
        {
            node->left->parent = node->parent;
            if (tree->compFunc(node->parent->right->data, node->data) == 0)
            {
                node->parent->right = node->left;
            }
            else
            {
                node->parent->left = node->left;
            }
        }
        node->left->color = BLACK;
    }
    tree->freeFunc(node->data);
    free(node);
    node = NULL;
}

/**
 * @brief the first case- if the node is red both of its kids are null (if not- then the black
 * length to its kids will be different
 * @param tree the tree
 * @param node the node to delete
 */
void delete1(RBTree* tree, Node* node)
{
    if(node->parent->right != NULL && tree->compFunc(node->parent->right->data, node->data) == 0)
    {
        node->parent->right = NULL;
    }
    else
    {
        node->parent->left = NULL;
    }
    tree->freeFunc(node->data);
    free(node);
    node = NULL;
}

/**
 * @brief finds a node in the tree
 * @param tree the tree
 * @param data the data of the node to find
 */
Node* nodeFinder(RBTree * tree, Node * node, void* data)
{
    if(tree->compFunc(node->data, data) == 0 || node == NULL)
    {
        return node;
    }
    else if(tree->compFunc(node->data, data) < 0)
    {
        return nodeFinder(tree, node->right, data);
    }
    else
    {
        return nodeFinder(tree, node->left, data);
    }
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
    if(tree == NULL)
    {
        return 0;
    }
    return contains(tree, tree->root, data);
}

/**
 * @brief helper function for RBTreeContains
 * @param node the node to check
 * @param data the data to check
 * @return 1 if the tree does contain the data 0 if not
 */
int contains(const RBTree *tree, Node* node, const void * data)
{
    if(node != NULL)
    {
        if(tree->compFunc(node->data, data) == 0)
        {
            return 1;
        }
        if(tree->compFunc(node->data, data) < 0)
        {
            return contains(tree, node->right, data);
        }
        else
        {
            return contains(tree, node->left, data);
        }
    }
    else
    {
        return 0;
    }
}



/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if(tree == NULL)
    {
        return 0;
    }
    return forEachHelper(tree->root, func, args);
}

/**
 * @brief helps go over the nodes
 * @return 1 if successful 0 if not
 */
int forEachHelper(Node *node, forEachFunc func, void * args)
{
    if(node == NULL)
    {
        return 1;
    }
    int left = forEachHelper(node->left, func, args);
    int try = func(node->data, args);
    if(try == 0 || left == 0)
    {
        return 0;
    }
    int right = forEachHelper(node->right, func, args);
    if(right == 0)
    {
        return 0;
    }
    return 1;
}


/**
 * free all memory of the data structure.
 * @param tree the tree to free
 */
void freeRBTree(RBTree **tree)
{
    Node* node = (*tree)->root;
    freeHelper(node, (*tree)->freeFunc);
    free(*tree);
}

/**
 * @brief helps free the data
 * @param node the node to free
 */
void freeHelper(Node* node, FreeFunc func)
{
    if(node != NULL)
    {
        freeHelper(node->right, func);
        func(node->data);
        freeHelper(node->left, func);
        free(node);
    }
}