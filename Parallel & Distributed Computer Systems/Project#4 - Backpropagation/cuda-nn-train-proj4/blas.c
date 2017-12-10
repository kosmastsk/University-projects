//
// Created by achilleas on 5/10/2017.
//


#include <stdlib.h>
#include <stdio.h>
#include "blas.h"

void cuBlas_matmul(int TA, int TB, int M, int N, int K, float ALPHA,
              float *A_gpu, int lda,
              float *B_gpu, int ldb,
              float BETA,
              float *C_gpu, int ldc)
{
    cublasHandle_t handle;
    stat = cublasCreate(&handle);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf ("CUBLAS initialization failed\n");
        return exit(1);
    }

    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N),
                                     (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    if(status!=CUBLAS_STATUS_SUCCESS){
        printf("%s", status);
        exit(1);
    }

}