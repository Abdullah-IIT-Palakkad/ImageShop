#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "image.h"
// MACROS
#define MAX_LINE_SIZE 70

int main(int argc, char *argv[]){
    
    char *in, *out, *T1, *T2;

    /*  arg parser
        format : ./ImageShop inputfilepath outputfilepath T1:(int)0/1 T2:(int)0/1
    */
    if(argc != 5){
        printf("ERROR: main: Incorrect number(%d) of arguments passed, expected 4\n", argc);
        exit(1);
    }
    in = argv[1];
    out = argv[2];
    T1 = argv[3];
    T2 = argv[4];
    // Input file
    FILE* iptr = fopen(in,"r");
    if(iptr == NULL){
        perror("ERROR: main");
        exit(1);
    }
    // output file
    FILE* optr = fopen(out,"w");
    if(optr == NULL){
        perror("ERROR: main:");
        exit(1);
    }

    char istream[MAX_LINE_SIZE];
    int iheight, iwidth, imaxval;
    fscanf(iptr, "%s", istream);
    if(istream[0] != 'P' || istream[1] != '3'){
        printf("ERROR: main: Invalid file format");
        exit(1);
    }
    // removing comments
    char c = getc(iptr);
    while (c == '#') {
    while (getc(iptr) != '\n') ;
         c = getc(iptr);
    }
    ungetc(c, iptr);

    fscanf(iptr, "%d %d", &iwidth, &iheight);
    fscanf(iptr, "%d", &imaxval);

    // Image read
    image* img = initIMAGE(istream, iheight, iwidth, imaxval);
    loadIMAGE(img, iptr);
    
    // Transformations
    if(T1[0] == '1')
        img = Transform1(img);
    if (T2[0] == '1')
        img = Transform2(img);

    // Image write
    unloadIMAGE(img, optr);
    
    return 0;
}