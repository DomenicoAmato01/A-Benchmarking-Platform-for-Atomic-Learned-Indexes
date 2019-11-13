/*
*
*   Query Script using Intel Math Kernel for optimized Matrix Multiplication on intel CPU
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <algorithm>

#include "lib/util.h"

int prediction(const char *fn, const char *wfn1,const char *bfn1, const char *output,const char* dataName, const char * query){
    
    double *A, *B, *C, *bias;
    int m, n, k, i, j;
    double alpha, beta;

    struct timespec requestStart, requestEnd;
    double* timer;

    float perc = 1;

    FILE* out_fp;
    bool flagExist = true;

    out_fp = fopen( output, "w+" );
    if(out_fp == NULL || fgetc(out_fp) == EOF){
        flagExist = false; 
    }
    fclose(out_fp);
 
    out_fp = fopen( output, "a+" );
    if(out_fp == NULL){ 
        std::stringstream ss;
        ss << "Opening File Error: " << errno;
        
        throw ss.str();
    }

    if(!flagExist){
       fprintf(out_fp, "Dataset, Query, #Elementi, Mult Time(s), Bias Time (s), Relu Time(s), Tot Time(s), Mean Time(s)\n"); 
    }

    //Binary Resolution and number of neurons 
    k = 64, n = 1;
    //Multiplication Coefficent C=alpha*A*B+beta*C
    alpha = 1.0; beta = 0.0;

    std::cout << "Reading matrix file..." << std::endl;

    try{
        m = readMatrix(fn, &A, k, perc);
        readNNParams(wfn1, &B, k);
        readNNBias(bfn1, &bias, m, n);
    }catch(std::string msg){//const char* msg){

        std::cerr << msg << std::endl;
        return -1;
    }

    std::cout << "Allocating memory for Result matrix.."<< std::endl;
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
        std::cerr << "Allocation Result Matrix Failed. Aborting..." << std::endl;
        mkl_free(A);
        mkl_free(B);
        mkl_free(C);
        return -1;
    }

    //Initializing Result Matrix
    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    //Allocating times Array
    timer = (double*)malloc(sizeof(double)*3);
    
    std::cout << " Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface..." << std::endl;
    clock_gettime(CLOCK_REALTIME, &requestStart);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    clock_gettime(CLOCK_REALTIME, &requestEnd);
    printf ("\n Computations completed.\n\n");

    timer[0] = (double)( requestEnd.tv_sec - requestStart.tv_sec )
    + (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
    / RES;

    std::cout << " Summing Bias..." << std::endl;
    clock_gettime(CLOCK_REALTIME, &requestStart);
    vdAdd(m, C, bias, C );
    clock_gettime(CLOCK_REALTIME, &requestEnd);
    timer[1] = (double)( requestEnd.tv_sec - requestStart.tv_sec )
        + (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
        / RES;

    std::cout << " Computing LeakyRelu Activation..." << std::endl;
    clock_gettime(CLOCK_REALTIME, &requestStart);
    for(i=0; i<m; i++){
        C[i] = C[i]>0 ? C[i] : C[i]*0.05;
    }
    clock_gettime(CLOCK_REALTIME, &requestEnd);
    timer[2] = (double)( requestEnd.tv_sec - requestStart.tv_sec )
        + (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
        / RES;

    std::cout << "Deallocating memory" << std::endl;
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    double tot = 0;
    for(int t=0; t<3; t++){
        tot +=timer[t];
    }

    std::cout << "Saving Result Time" << std::endl;
	fprintf(out_fp, "%s,%s,%d,%1.9lf,%1.9lf,%1.9lf,%1.9lf,%1.9lf\n" , dataName, query, m, timer[0], timer[1], timer[2], tot, tot/m);

	fclose(out_fp);

    return 0;
}

int main(int argc, char * argv[]){

    char *dataName, *queryName, *outputFn;

    std::cout << "Check Parameters..." << std::endl;

    //Print help
    if(cmdOptionExists(argv, argv+argc, "-h"))
    {
        std::cout << "Help"<< std::endl;
        return 0;
    }

    //Check Dataset name parameter
    if(!cmdOptionExists(argv, argv+argc, "-d"))
    {
        std::cerr << "Dataset name missed...Aborting..." << std::endl;
        return 1;
    }else{
        dataName = getCmdOption(argv, argv + argc, "-d");
        if(!dataName || !strncmp ( dataName, "-", 1 )){
            std::cerr << "Dataset name missed...Aborting..." << std::endl;
            return 1;
        }
    }

    //Check Query name parameter
    if(!cmdOptionExists(argv, argv+argc, "-q"))
    {
        std::cerr << "Query name missed...Aborting..." << std::endl;
        return 1;
    }else{
        queryName = getCmdOption(argv, argv + argc, "-q");
        if(!queryName || !strncmp ( queryName, "-", 1 )){
            std::cerr << "Query name missed...Aborting..." << std::endl;
            return 1;
        }
    }

    //Check Output filename parameter
    if(!cmdOptionExists(argv, argv+argc, "-o"))
    {
        std::cerr << "Output filename missed...Aborting..." << std::endl;
        return 1;
    }else{
        outputFn = getCmdOption(argv, argv + argc, "-o");
        if(!outputFn || !strncmp ( outputFn, "-", 1 )){
            std::cerr << "Output filename missed...Aborting..." << std::endl;
            return 1;
        }
    }

    std::stringstream ss;
    std::string fn, wFn, bFn, oFn, qFn;
    ss.str("");
    ss.clear();

    ss << dataName << "Query" << queryName << "_bin.dat";
    fn = ss.str();
    ss.str("");
    ss.clear();


    ss << "weights_" << dataName << ".dat";
    wFn = ss.str();
    ss.str("");
    ss.clear();

    ss << "bias_" << dataName << ".dat";
    bFn = ss.str();
    ss.str("");
    ss.clear();

    std::cout << "Starting Prediction..." << std::endl;
    prediction(fn.c_str(), wFn.c_str(), bFn.c_str(), outputFn, dataName, queryName);

    




}

