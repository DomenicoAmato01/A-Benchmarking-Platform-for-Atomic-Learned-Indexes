/*
*
*   Query Script using Intel Math Kernel for optimized Matrix Multiplication on intel CPU
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "lib/nn.h"

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

