#include <iostream>
#include <time.h>
#include <curand.h>
#include <cublas_v2.h>
#include <cuda_fp16.h>
#include <algorithm>

#define RES 1e9


/*
* Argument Parser
*/
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

/*
* Matrix Utility
*/
long readMatrix(const char* fn, float **data, int m){

    FILE* fp;
    long num = 0;
    fp = fopen( fn, "r" );
    if(fp == NULL) std::cout << "Errore apertura file: " << errno << std::endl;

    std::cout <<"Leggo il numero di righe" << std::endl;
    fscanf(fp, "%ld\n", &num);

    std::cout << "Alloco " << num << std::endl;
    *data = (float*)malloc(sizeof(float)*(num*m));
    std::cout << "Leggo i valori" << std::endl;
    int aus = 0;
    for(int i = 0; i<num; i++){
        for(int j = 0; j<m;j++){
            fscanf(fp, "%1d", &aus);
            data[0][j * num + i] = (float)aus;
            //std::cout << data[0][j * num + i];
        }
        //std::cout << std::endl;
        //fgetc(fp);
    }
    //printf("A[]")
    fclose(fp);
    std::cout << "Ritorno numero di righe" << std::endl;
    return num;
}

void readNNParams(const char* fn, float **data, int m){

    FILE* fp;
    fp = fopen( fn, "r" );
    if(fp == NULL) std::cout << "Errore apertura file: " << errno << std::endl;
    //printf("Leggo Parametri");
    std::cout << "Alloco Parametri" << std::endl;
    *data = (float*)malloc(sizeof(float)*(m));
    std::cout << "Leggo i valori" << std::endl;
    float aus = 0.0;
    for(int j = 0; j<m;j++){
        fscanf(fp, "%f\n", &aus);
        data[0][j] = aus;
        //std::cout << data[0][j] << std::endl;
    }


    fclose(fp);
}

/*
* Cuda Operations
*/

// Multiply the arrays A and B on GPU and save the result in C
// C(m,n) = A(m,k) * B(k,n)
void gpu_blas_mmul(cublasHandle_t &handle, const float *A, const float *B, float *C, const int m, const int k, const int n) {
	int lda=m,ldb=k,ldc=m;
	const float alf = 1;
	const float bet = 0;
	const float *alpha = &alf;
	const float *beta = &bet;
	//Do the actual multiplication
	//cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	cublasGemmEx(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, alpha,
		                          A, CUDA_R_32F, lda,
		                          B, CUDA_R_32F, ldb,
								  beta, C, CUDA_R_32F, ldc, CUDA_R_32F, CUBLAS_GEMM_DEFAULT);
}
/*
void tpu_blas_mmul(cublasHandle_t &handle, const float *A, const float *B, float *C, const int m, const int k, const int n) {
	int lda=m,ldb=k,ldc=m;
	const float alf = 1;
	const float bet = 0;
	const float *alpha = &alf;
	const float *beta = &bet;

	//Do the actual multiplication
	//cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	cublasGemmEx(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, alpha,
	                          A, CUDA_R_32F, lda,
	                          B, CUDA_R_32F, ldb,
	                          beta, C, CUDA_R_32F, ldc, CUDA_R_32F, CUBLAS_GEMM_DEFAULT_TENSOR_OP);
}
*/
__global__ void k_relu(float *a, float *c)
{
    int idx = blockIdx.x;
    c[idx] = a[idx]>0 ? a[idx] : a[idx]*0.05;
}

__global__ void k_scalar_matrix_sum(float *a, float *b, float *c)
{
    int idx = blockIdx.x;
    c[idx] = a[idx] + *b;
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

/*
* Prediction Function
*/

void prediction(const char *fn,const char *wfn1,const char *bfn1, const char *output){
	
	//Name of data file
	//const char *fn="Resource/lognormal/Query/file3lognormalQuery10_bin_c.dat";
	//const char *wfn1="Resource/lognormal/weights_dat/weights_nn1_file3_c.dat";
	//const char *bfn1="Resource/lognormal/weights_dat/bias_nn1_file3_c.dat";


	int nr_rows_A, nr_cols_A,
	nr_rows_W1, nr_cols_W1,
	nr_rows_C1, nr_cols_C1,
	nr_bias1;

	nr_bias1 = nr_cols_W1 = 1;

	float *h_A;
	float *h_W1;
	float *h_bias1;

	struct timespec requestStart, requestEnd;
	double timer;
	double copy_acc=0, mult_acc=0, bias_acc=0, relu_acc=0;

	// Create a handle for CUBLAS
	cublasHandle_t handle;
	cublasStatus_t cublasStat =  cublasCreate(&handle);

	FILE* out_fp;
    out_fp = fopen( output, "a" );
    if(out_fp == NULL) std::cout << "Errore apertura file Output: " << errno << std::endl;

	nr_cols_A = nr_rows_W1 = 64;
	nr_rows_A = (int)readMatrix(fn, &h_A, nr_cols_A);

	nr_rows_C1 = nr_rows_A;
	nr_cols_C1 = nr_cols_W1;
	readNNParams(wfn1, &h_W1, nr_cols_A*nr_cols_W1);
	readNNParams(bfn1, &h_bias1, nr_bias1);

/*
	std::cout << "A =" << std::endl;
	print_matrix(h_A, nr_rows_A, nr_cols_A);

	std::cout << "W =" << std::endl;
	print_matrix(h_W1, nr_rows_W1, nr_cols_W1);
*/
	float *h_C1 = (float *)malloc(nr_rows_C1 * nr_cols_C1 * sizeof(float));

	// Allocate 3 arrays on GPU
	float *d_A, *d_W1, *d_bias1;
	float *d_C1;
	cudaMalloc(&d_A,nr_rows_A * nr_cols_A * sizeof(float));
	cudaMalloc(&d_W1,nr_rows_W1 * nr_cols_W1 * sizeof(float));
	cudaMalloc(&d_bias1,nr_bias1 * sizeof(float));
	cudaMalloc(&d_C1,nr_rows_C1 * nr_cols_C1 * sizeof(float));


	clock_gettime(CLOCK_REALTIME, &requestStart);
	cudaMemcpy(d_A,h_A,nr_rows_A * nr_cols_A * sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(d_W1,h_W1,nr_rows_W1 * nr_cols_W1 * sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(d_bias1,h_bias1,nr_bias1 * sizeof(float),cudaMemcpyHostToDevice);
	cudaDeviceSynchronize();
	/*clock_gettime(CLOCK_REALTIME, &requestEnd);
	timer = (double)( requestEnd.tv_sec - requestStart.tv_sec )
	+ (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
	/ RES;
	std::cout << "Time Copy Input TPU: " << timer << std::endl;
	copy_acc += timer;
*/
	// Set the math mode to allow cuBLAS to use Tensor Cores:
	cublasStat = cublasSetMathMode(handle, CUBLAS_TENSOR_OP_MATH);
	//clock_gettime(CLOCK_REALTIME, &requestStart);
	// Multiply A and B on GPU
	tpu_blas_mmul(handle, d_A, d_W1, d_C1, nr_rows_A, nr_cols_A, nr_cols_W1);
	cudaDeviceSynchronize();
	/*clock_gettime(CLOCK_REALTIME, &requestEnd);
	timer = (double)( requestEnd.tv_sec - requestStart.tv_sec )
	+ (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
	/ RES;
	std::cout << "Time Mult TPU: " << timer << std::endl;
	mult_acc += timer;

	clock_gettime(CLOCK_REALTIME, &requestStart);*/
	//Bias sum
	k_scalar_matrix_sum<<<nr_rows_A*nr_cols_W1, 1>>>(d_C1, d_bias1, d_C1);
	cudaDeviceSynchronize();
	/*clock_gettime(CLOCK_REALTIME, &requestEnd);
	timer = (double)( requestEnd.tv_sec - requestStart.tv_sec )
	+ (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
	/ RES;
	std::cout << "Time Bias GPU: " << timer << std::endl;
	bias_acc += timer;

	clock_gettime(CLOCK_REALTIME, &requestStart);*/
	//Bias sum
	k_relu<<<nr_rows_A*nr_cols_W1, 1>>>(d_C1, d_C1);
	cudaDeviceSynchronize();
	/*clock_gettime(CLOCK_REALTIME, &requestEnd);
	timer = (double)( requestEnd.tv_sec - requestStart.tv_sec )
	+ (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
	/ RES;
	std::cout << "Time Relu GPU: " << timer << std::endl;
	relu_acc += timer;

	clock_gettime(CLOCK_REALTIME, &requestStart);*/
	// Copy (and print) the result on host memory
	cudaMemcpy(h_C1,d_C1,nr_rows_C1 * nr_cols_C1 * sizeof(float),cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	timer = (double)( requestEnd.tv_sec - requestStart.tv_sec )
	+ (double)( requestEnd.tv_nsec - requestStart.tv_nsec )
	/ RES;
	std::cout << "Time Copy Output TPU: " << timer << std::endl;
	copy_acc+= timer;
	//std::cout << "C =" << std::endl;
	//print_matrix_float(h_C, nr_rows_C, nr_cols_C);
	//print_matrix_float(h_A, nr_rows_A, nr_cols_A);


	// Free CPU memory
	free(h_A);
	free(h_W1);
	free(h_bias1);
	free(h_C1);


	//Free GPU memory
	cudaFree(d_A);
	cudaFree(d_W1);
	cudaFree(d_bias1);
	cudaFree(d_C1);
	cudaDeviceSynchronize();

	//printf("Scrivo Risultati\n");
	//fprintf(out_fp, "%1.9lf,%1.9lf,%1.9lf,%1.9lf\n" , copy_acc, mult_acc, bias_acc, relu_acc);

	//fclose(out_fp);
	// Destroy the handle
	//cublasDestroy(handle);
}

/*
* Main Program
*/

int main(int argc, char * argv[]) {

	

	char *fullName, *weights_full, *bias_full, *fullNameRes;
	const char *pathQuery = "./Resource/lognormal/Query/", *pathWeights = "./Resource/lognormal/weights_dat/", *pathOutput = "./Result/lognormal/csv/KerasExperiment2/cudaQuery/";
	char inum[100],jnum[100];
	int file[4] = {3, 7,10, 13};
	int perc[3] = {10, 50, 80};

	for(int i = 0; i < 4; i++){
		std::cout << "Iterazione:" << i << std::endl;
		std::cout << "Apertura File" << file[i] << std::endl; 
		/*
		* Generazione file name dei pesi
		*/
		weights_full = (char*)malloc(sizeof(char)*1000);
		weights_full[0] = '\0';
		strcat(weights_full, pathWeights);
		sprintf(inum, "weights_nn1_file%d_c.dat", file[i]);
		strcat(weights_full, inum);

		/*
		* Generazione file name dei bias
		*/
		bias_full = (char*)malloc(sizeof(char)*1000);
		bias_full[0] = '\0';
		strcat(bias_full, pathWeights);
		sprintf(inum, "bias_nn1_file%d_c.dat", file[i]);
		strcat(bias_full, inum);
		
		printf("%s\n", weights_full);
		printf("%s\n", bias_full);
		if(i < 3){
			std::cout << "Sono nell'IF" << std::endl;
			for(int j = 0; j< 3; j++){
				
				/*
				* Generazione file name delle query
				*/
				fullName = (char*)malloc(sizeof(char)*1000);
				fullName[0] = '\0';
				strcat(fullName, pathQuery);
				sprintf(inum, "file%dlognormalQuery", file[i]);
				strcat(fullName, inum);
				sprintf(jnum, "%d_bin_c.dat", perc[j]);
				strcat(fullName, jnum);


				printf("%s\n", fullName);

				/*
				* Generazione file name dei risultati
				*/
				fullNameRes = (char*)malloc(sizeof(char)*1000);
				fullNameRes[0] = '\0';
				strcat(fullNameRes, pathOutput);
				sprintf(inum, "file%dlognormal", file[i]);
				strcat(fullNameRes, inum);
				sprintf(jnum, "PredTime.csv");
				strcat(fullNameRes, jnum);


				printf("%s\n", fullNameRes);

				
				prediction(fullName, weights_full, bias_full, fullNameRes);
                printf("END PREDICTION");
				/*
				*Pulisco i file dei nomi
				*/
				free(fullName);
				free(fullNameRes);
			}
		}else{
			std::cout << "Sono nell'ELSE" << std::endl;
			for(int j = 0; j< 3; j++){
				
				/*
				* Generazione file name delle query
				*/
				int flag = 1;
				int part = 1;
				while(flag){//for(int part = 1; part<=6;part++){
					fullName = (char*)malloc(sizeof(char)*1000);
					fullName[0] = '\0';
					strcat(fullName, pathQuery);
					sprintf(inum, "file%dlognormalPart%dQuery", file[i], part);
					strcat(fullName, inum);
					sprintf(jnum, "%d_bin_c.dat", perc[j]);
					strcat(fullName, jnum);


					printf("%s\n", fullName);

					/*
					* Generazione file name dei risultati
					*/
					fullNameRes = (char*)malloc(sizeof(char)*1000);
					fullNameRes[0] = '\0';
					strcat(fullNameRes, pathOutput);
					sprintf(inum, "file%dlognormalQuery", file[i]);
					strcat(fullNameRes, inum);
					sprintf(jnum, "%dPredTime.csv", perc[j]);
					strcat(fullNameRes, jnum);


					printf("%s\n", fullNameRes);

					if(fopen(fullName, "r")){
						printf("il file esiste\n");
						prediction(fullName, weights_full, bias_full, fullNameRes);
					}else{

						printf("il file non esiste\n");
						std::cout << "Errore apertura file: " << errno << std::endl;
						flag = 0;
					}

					/*
					*Pulisco i file dei nomi
					*/
					free(fullName);
					free(fullNameRes);
					part++;
				}
			}
		}

		free(weights_full);
		free(bias_full);
	}

	/*const char *fn="Resource/lognormal/Query/file3lognormalQuery10_bin_c.dat";
	const char *wfn1="Resource/lognormal/weights_dat/weights_nn1_file3_c.dat";
	const char *bfn1="Resource/lognormal/weights_dat/bias_nn1_file3_c.dat";*/
	

    return 0;
}