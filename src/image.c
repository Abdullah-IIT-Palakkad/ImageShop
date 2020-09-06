#include "image.h"

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
                    printf("Dimension mismatch during multiplication");
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
                printf("Dimension mismatch during multiplication");
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