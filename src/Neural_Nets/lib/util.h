/*
*
*   Utility Library
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <algorithm>

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


long readMatrix(const char* fn, double **data, int m, float perc){

    FILE* fp; 
    long num = 0;
    fp = fopen( fn, "r" );
    if(fp == NULL){
        std::stringstream ss;
        ss << "Opening File Error: " << errno;
        
        throw ss.str();
    } 

    fscanf(fp, "%ld\n", &num);

    *data = (double*)mkl_malloc(sizeof(double)*(perc*num*m), 64);

    if(data[0] == NULL){
        std::stringstream ss;
        ss << "Allocation InputData Failed. Aborting...";
        
        throw ss.str();
    }

    for(int i = 0; i<perc*num; i++){
        for(int j = 0; j<64;j++){
            fscanf(fp, "%1lf", &data[0][i*m+j]);
        }
        fgetc(fp);
    }

    fclose(fp);
    return perc*num;
}

void readNNParams(const char* fn, double **data, int m){

    FILE* fp;
    fp = fopen( fn, "r" );
    if(fp == NULL){
        std::stringstream ss;
        ss << "Opening File Error: " << errno;
        
        throw ss.str();
    } 

    *data = (double*)mkl_malloc(sizeof(double)*(m), 64);

    if(data[0] == NULL){
        std::stringstream ss;
        ss << "Allocation Weights Failed. Aborting...";
        
        throw ss.str();
    }

    for(int j = 0; j<m;j++){
        fscanf(fp, "%lf\n", &data[0][j]);
    }
    fclose(fp);
}

void readNNBias(const char* fn, double **data, int n, int m){

    FILE* fp;
    fp = fopen( fn, "r" );

    if(fp == NULL){
        std::stringstream ss;
        ss << "Opening File Error: " << errno;
        
        throw ss.str();
    } 

    *data = (double*)mkl_malloc(sizeof(double)*(n*m), 64);

    if(data[0] == NULL){
        std::stringstream ss;
        ss << "Allocation Bias Failed. Aborting...";
        
        throw ss.str();
    }
         
    for(int j = 0; j<m;j++){
        fscanf(fp, "%lf\n", &data[0][j]);
        for(int i=0; i<n; i++){
            if(i != 0)
                data[0][i*m+j]=data[0][i*m+j-1];
        }
    }
    fclose(fp);
}

//Print matrix A(nr_rows_A, nr_cols_A) storage in column-major format
void print_matrix(const float *A, int nr_rows_A, int nr_cols_A) {

	for(int i = 0; i < nr_rows_A; ++i){
		for(int j = 0; j < nr_cols_A; ++j){
		   std::cout << A[j * nr_rows_A + i] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_matrix_float(const float *A, int nr_rows_A, int nr_cols_A) {

	for(int i = 0; i < nr_rows_A; ++i){
		for(int j = 0; j < nr_cols_A; ++j){
		   std::cout << A[j * nr_rows_A + i] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}