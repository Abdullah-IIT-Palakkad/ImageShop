#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    int r, c;
    int **data;
} matrix;

void allocMATRIX(matrix* m, int rows, int cols);
matrix* initMATRIX(int rows, int cols);
void loadMATRIX(matrix* m, int *entries);
matrix* loadWindow(matrix* m, int startX, int startY, int size);
int dot(matrix* P, matrix* A, matrix* B);
int multiply(matrix* P, matrix* A, matrix* B);
int elementSum(matrix* P, float normalizing_factor);