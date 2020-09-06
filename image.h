#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

typedef struct
{
    int rows, cols, maxval;
    char *format;
    matrix** color;
} image;

image* initIMAGE(char *imgformat, int rows, int cols, int maxval);
void loadIMAGE(image* img, FILE* fptr);
void unloadIMAGE(image* img, FILE* fptr);
image* Transform1(image* img);
image* Transform2(image* img);