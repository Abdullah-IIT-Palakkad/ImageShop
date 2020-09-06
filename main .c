#include <stdio.h>
#include <stdlib.h>

// MACROS
#define MAX_LINE_SIZE 70

// STRUCTS
typedef struct 
{
    int r, c;
    int **data;
} matrix;

typedef struct
{
    int rows, cols, maxval;
    char *format;
    matrix** color;
} image;


// FUNCTION DECLARATIONS
void print_err(char *err_msg);
void printMatrix(matrix* A);

// MATRIX
void allocMATRIX(matrix* m, int rows, int cols);
matrix* initMATRIX(int rows, int cols);
void loadMATRIX(matrix* m, int *entries);
matrix* loadWindow(matrix* m, int startX, int startY, int size);
int dot(matrix* P, matrix* A, matrix* B);
int multiply(matrix* P, matrix* A, matrix* B);
int elementSum(matrix* P, float normalizing_factor);

// IMAGE
image* initIMAGE(char *imgformat, int rows, int cols, int maxval);
void loadIMAGE(image* img, FILE* fptr);
void unloadIMAGE(image* img, FILE* fptr);
image* Transform1(image* img);
image* Transform2(image* img);

int main(int argc, char *argv[]){
    
    char *filename;
    int iheight, iwidth, imaxval;

    /*  arg parser
        format : executable filename Transform1 Transform2
    */
    if (argc == 2)
    {
        filename = argv[1];
    } else if (argc > 2)
    {
        print_err("Too many arguments passed");
    } else
    {
        filename = "img.ppm";
    }


    FILE* fptr = fopen(filename,"r");
    if(fptr == NULL)
    {
        print_err("Invalid input file path");
    }

    FILE* optr = fopen("output.ppm","w");
    if(fptr == NULL)
    {
        print_err("Invalid output file path");
    }
    
    char istream[MAX_LINE_SIZE];
    fscanf(fptr, "%s", istream);
    if(istream[0] != 'P' || istream[1] != '3'){
        print_err("Invalid file format");
    }
    fscanf(fptr, "%d %d", &iwidth, &iheight);
    fscanf(fptr, "%d", &imaxval);
    
    image* img = initIMAGE(istream, iheight, iwidth, imaxval);
    loadIMAGE(img, fptr);
    img = Transform2(img);
    img = Transform1(img);
    unloadIMAGE(img, optr);
    return 0;
}

// FUNCTION DEFINITIONS
void print_err(char *err_msg)
{
    printf("ERROR: %s\n", err_msg);
    exit(1);
}


// MATRIX
void allocMATRIX(matrix* m, int rows, int cols){
    m->r = rows;
    m->c = cols;

    m->data = (int **) malloc(rows * sizeof(int *) );
    for (int i = 0; i<rows; i++){
        m->data[i] = (int *) malloc(cols * sizeof(int) );
    }
}

matrix* initMATRIX(int rows, int cols){
    matrix* NewMatrix = (matrix* ) malloc( sizeof(matrix) );
    allocMATRIX(NewMatrix, rows, cols);

    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            NewMatrix->data[i][j] = 0;
        }
    }

    return NewMatrix;
}

void loadMATRIX(matrix* m, int *entries){
    int idx = 0;
    for(int i = 0; i < m->r; i++){
        for(int j = 0; j < m->c; j++){
            m->data[i][j] = entries[idx];
            idx++;
        }
    }
}

matrix* loadWindow(matrix* m, int startX, int startY, int size){
    matrix* temp = initMATRIX(size, size);
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            temp->data[i][j] = m->data[startX+i-1][startY+j-1];
        }
    }
    return temp;
}

int multiply(matrix* P, matrix* A, matrix* B){
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

int dot(matrix* P, matrix* A, matrix* B){
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

int elementSum(matrix* P, float normalizing_factor){
    int res = 0;
    for(int i = 0; i < P->r; i++){
        for(int j = 0; j < P->c; j++){
            res += P->data[i][j];
        }
    }
    res = abs(res);
    return res/normalizing_factor;
}


// IMAGE
image* initIMAGE(char *imgformat, int rows, int cols, int maxval)
{
    image* NewImage = (image *) malloc( sizeof(image) );
    NewImage->color = (matrix **) malloc( 3 * sizeof(matrix *) );
    NewImage->format = (char *) malloc(2 * sizeof(char) );
    NewImage->rows = rows;
    NewImage->cols = cols;
    NewImage->maxval = maxval;
    NewImage->format[0] = imgformat[0];
    NewImage->format[1] = imgformat[1];

    for (int i = 0; i < 3; i++){
        NewImage->color[i] = initMATRIX(rows, cols);
    }
    return NewImage;
}

void loadIMAGE(image* img, FILE* fptr){
    int r, g, b;
    for(int i = 0; i < img->rows; i++){
        for(int j = 0; j < img->cols; j++){
            fscanf(fptr, "%d %d %d", &r, &g, &b);
            img->color[0]->data[i][j] = r;
            img->color[1]->data[i][j] = g;
            img->color[2]->data[i][j] = b;
        }
    }
}

void unloadIMAGE(image* img, FILE* fptr){
    fprintf(fptr, "%s\n", img->format);
    fprintf(fptr, "%d %d\n%d\n", img->cols, img->rows, img->maxval);
    for(int i = 0; i < img->rows; i++){
        for(int j = 0; j < img->cols; j++){
            fprintf(fptr, "%d %d %d ", img->color[0]->data[i][j], img->color[1]->data[i][j], img->color[2]->data[i][j]);
        }
        fprintf(fptr, "\n");
    }
}

image* Transform1(image* img){
    image* temp = initIMAGE(img->format, img->rows, img->cols, img->maxval);

    matrix* kernel = initMATRIX(3,3);
    int edge[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
    loadMATRIX(kernel, edge);
    for(int comp = 0; comp < 3; comp++){
        for(int i = 1; i < img->rows-1; i++){
            for(int j = 1; j < img->cols-1; j++){
                matrix* window = loadWindow(img->color[comp], i, j, kernel->r);
                matrix* P = (matrix* ) malloc( sizeof(matrix) );
                int ret_val = multiply(P, kernel, window);
                // int ret_val = dot(P, kernel, window);
                if(ret_val != 0){
                    print_err("Dimension mismatch during multiplication");
                }
                // printf("\n");
                // printMatrix(P);
                temp->color[comp]->data[i][j] = elementSum(P, 1);
            }
        }
    }
    return temp;
}

image* Transform2(image* img){
    int new_rows = (img->cols%2 == 0) ? img->cols+1 : img->cols;
    int new_cols = (img->rows%2 == 0) ? img->rows+1 : img->rows;
    image* temp = initIMAGE(img->format, new_rows, new_cols, img->maxval);

    matrix* affine = initMATRIX(2,2);
    int edge[] = {0,-1,1,0};
    loadMATRIX(affine, edge);
    for(int i = 0; i < img->rows; i++){
        for(int j = 0; j < img->cols; j++){
            matrix* pos_vector = initMATRIX(2, 1);
            int pos[] = {i - img->rows/2, j - img->cols/2};
            loadMATRIX(pos_vector, pos);
            matrix* P = (matrix* ) malloc( sizeof(matrix) );
            int ret_val = multiply(P, affine, pos_vector);
            // int ret_val = dot(P, kernel, window);
            if(ret_val != 0){
                print_err("Dimension mismatch during multiplication");
            }
            // printf("\n");
            // printMatrix(affine);
            int new_i = P->data[0][0] + temp->rows/2;
            int new_j = P->data[1][0] + temp->cols/2;
            int curr_r = img->color[0]->data[i][j];
            int curr_g = img->color[1]->data[i][j];
            int curr_b = img->color[2]->data[i][j];

            temp->color[0]->data[new_i][new_j] = curr_r;
            temp->color[1]->data[new_i][new_j] = curr_g;
            temp->color[2]->data[new_i][new_j] = curr_b;
        }
    }
    return temp;
}

void printMatrix(matrix* A){
    for(int i = 0; i < A->r; i++){
        for(int j = 0; j < A->c; j++){
            printf("%d ", A->data[i][j]);
        }
        printf("\n");
    }
}



/*
    for(int i = 0; i < iheight; i++){
        for(int j = 0; j < iwidth; j++){
            printf("(%f, %f, %f) ", img->color[0]->data[i][j], img->color[1]->data[i][j], img->color[2]->data[i][j]);
        }
        printf("\n");
    }
    */