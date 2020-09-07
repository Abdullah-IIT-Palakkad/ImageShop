#include "matrix.h"

void allocMATRIX(matrix m, int rows, int cols){
    m->r = rows;
    m->c = cols;

    m->data = (int **) malloc(rows * sizeof(int *) );
    for (int i = 0; i<rows; i++){
        m->data[i] = (int *) malloc(cols * sizeof(int) );
    }
}

matrix initMATRIX(int rows, int cols){
    matrix NewMatrix = (matrix ) malloc( sizeof(matrix_t) );
    allocMATRIX(NewMatrix, rows, cols);

    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            NewMatrix->data[i][j] = 0;
        }
    }

    return NewMatrix;
}

void loadMATRIX(matrix m, int *entries){
    int idx = 0;
    for(int i = 0; i < m->r; i++){
        for(int j = 0; j < m->c; j++){
            m->data[i][j] = entries[idx];
            idx++;
        }
    }
}

matrix loadWindow(matrix m, int startX, int startY, int size){
    matrix temp = initMATRIX(size, size);
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            temp->data[i][j] = m->data[startX+i-1][startY+j-1];
        }
    }
    return temp;
}

int multiply(matrix P, matrix A, matrix B){
    // dim check
    if(A->c != B->r){
        return 1;
    }
    // initializing P
    allocMATRIX(P, A->r, B->c);
    // Multiplying
    for(int i = 0; i < P->r; i++){
        for(int j = 0; j < P->c; j++){
            for(int k = 0; k < A->c; k++){
                P->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }
    return 0;
}

int dot(matrix P, matrix A, matrix B){
    if(A->r != B->r || A->c != B->c ){
        return 1;
    }
    allocMATRIX(P, A->r, B->c);
    for(int i = 0; i < A->r; i++){
        for(int j = 0; j < B->c; j++){
            P->data[i][j] = A->data[i][j] * B->data[i][j];
        }
    }
    return 0;
}

int elementSum(matrix P, float normalizing_factor){
    int res = 0;
    for(int i = 0; i < P->r; i++){
        for(int j = 0; j < P->c; j++){
            res += P->data[i][j];
        }
    }
    res = abs(res);
    return res/normalizing_factor;
}
