#include <stdio.h>
#include <stdlib.h>
#include "image.h"
// MACROS
#define MAX_LINE_SIZE 70

// STRUCTS

// FUNCTION DECLARATIONS
void print_err(char *err_msg);
void printMatrix(matrix* A);

// MATRIX

// IMAGE


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


// IMAGE


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