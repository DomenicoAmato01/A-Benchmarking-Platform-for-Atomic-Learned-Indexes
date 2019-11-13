/*
*
*   Utility Library
*
*/
#include <stdio.h>
#include <stdlib.h>

/*
*
*   Intel Math Kernel Library
*   [Change install directory path]
*
*/
#include "/opt/intel/mkl/include/mkl.h"
#include "/opt/intel/mkl/include/mkl_vml_functions.h"

//Time Resolution (1e9 is nanoseconds) 
#define RES 1e9

long readMatrix(char* fn, double **data, int m, float perc){

    FILE* fp; 
    long num = 0;
    fp = fopen( fn, "r" );

    fscanf(fp, "%ld\n", &num);

    *data = (double*)mkl_malloc(sizeof(double)*(perc*num*m), 64);

    for(int i = 0; i<perc*num; i++){
        for(int j = 0; j<64;j++){
            fscanf(fp, "%1lf", &data[0][i*m+j]);
        }
        fgetc(fp);
    }

    fclose(fp);
    return perc*num;
}

void readNNParams(char* fn, double **data, int m){

    FILE* fp;
    fp = fopen( fn, "r" );

    *data = (double*)mkl_malloc(sizeof(double)*(m), 64);

    for(int j = 0; j<m;j++){
        fscanf(fp, "%lf\n", &data[0][j]);
    }
    fclose(fp);
}

void readNNBias(char* fn, double **data, int n, int m){

    FILE* fp;
    fp = fopen( fn, "r" );

    *data = (double*)mkl_malloc(sizeof(double)*(n*m), 64);

    
         
    for(int j = 0; j<m;j++){
        fscanf(fp, "%lf\n", &data[0][j]);
        for(int i=0; i<n; i++){
            if(i != 0)
                data[0][i*m+j]=data[0][i*m+j-1];
        }
    }
    fclose(fp);
}