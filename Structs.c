/** @file Structs.c
* @author  Yair Escott <yair.95@gmail.com>
*
* @brief implementing Vectors
*/

// ------------------------------ includes ------------------------------
#include "Structs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
// -------------------------- const definitions -------------------------

//CompFunc for strings (assumes strings end with "\0")
int stringCompare(const void *a, const void *b);

// ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
int concatenate(const void *word, void *pConcatenated);

// FreeFunc for strings
void freeString(void *s);

// CompFunc for Vectors, compares element by element, the vector that has the first larger
int vectorCompare1By1(const void *a, const void *b);

// FreeFunc for vectors
void freeVector(void *pVector);

// copy pVector to pMaxVector
int copyIfNormIsLarger(const void *pVector, void *pMaxVector);

// This function allocates memory it does not free.
Vector *findMaxNormVectorInTree(RBTree *tree);

// calculates the norm of the vector
double normCalculator(Vector* vector);

// finds the minimum
int min(int num1, int num2);

// ------------------------------ functions -----------------------------

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return strcmp((char*) a, (char*) b);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if(word == NULL || pConcatenated == NULL)
    {
        return 0;
    }
    char* word2 = (char*)word;
    char* pConc = (char*) pConcatenated;
    strcat(word2, pConc);
    strcat(word2, "\n");
    return 1;
}

/**
 * @brief frees strings
 * @param s the string
 */
void freeString(void *s)
{
    free(s);
}

/**
 * @brief finds the minium
 * @param num1 the first number
 * @param num2 the second number
 * @return the minimum number
 */
int min(int num1, int num2)
{
    return (num1 > num2 ) ? num2 : num1;
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector* av = (Vector*) a;
    Vector* bv = (Vector*) b;
    int alen = av->len;
    int blen = bv->len;
    for(int i = 0; i<min(alen, blen); i++)
    {
        if(av->vector[i] < bv->vector[i])
        {
            return -1;
        }
        else if(av->vector[i] > bv->vector[i])
        {
            return 1;
        }
    }
    if(alen == blen)
    {
        return 0;
    }
    else if(alen < blen)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief frees a vector
 * @param pVector the vector to free
 */
void freeVector(void *pVector)
{
    Vector* vector = (Vector*) pVector;
    free(vector->vector);
    vector->vector = NULL;
    free(pVector);
    vector = NULL;
    pVector = NULL;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    Vector * first = (Vector*) pVector;
    Vector * second = (Vector*) pMaxVector;
    if(second->vector == NULL || normCalculator(first) > normCalculator(second))
    {
        if(first->len != second->len)
        {
            second->vector = realloc(second->vector, (first->len) * sizeof(double));
            if(second->vector == NULL)
            {
                return 0;
            }
        }
        for(int i = 0; i < first -> len; i++)
        {
            second->vector[i] = first->vector[i];
        }
        second->len = first->len;
    }
    return 1;
}

/**
 * @brief calculates the norm of the vector
 * @param vector the vector
 * @return the norm of the vector
 */
double normCalculator(Vector* vector)
{
    double sum = 0;
    for(int i = 0; i < vector->len; i++)
    {
        sum += pow(vector->vector[i], 2);
    }
    return sqrt(sum);
}

/**
 * This function allocates memory it does not free.
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector * vector = (Vector *)malloc(sizeof(Vector));
    vector->len = 0;
    vector->vector = NULL;
    int treeResult = forEachRBTree(tree, copyIfNormIsLarger, vector);
    if(treeResult == 0)
    {
        return NULL;
    }
    return vector;
}

