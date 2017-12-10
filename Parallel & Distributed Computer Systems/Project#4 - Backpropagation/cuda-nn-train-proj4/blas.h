//
// Created by achilleas on 2/10/2017.
//

//#include "cuda_utils.h"

#ifndef CUDA_NN_TRAIN_PROJ4_TRAINING_H
#define CUDA_NN_TRAIN_PROJ4_TRAINING_H



void our_matmul(float *A, float *B, float *C, int n, int m, int k); //our gemm sto blas_kernels.cu

void axpy_cuda(int N, float ALPHA, float *X, int INCX, float *Y, int INCY);
void cuBlas_matmul(int TA, int TB, int M, int N, int K, float ALPHA,
                   float *A_gpu, int lda,
                   float *B_gpu, int ldb,
                   float BETA,
                   float *C_gpu, int ldc);

void zero_init_gpu_array(int N, float *X);

void cross_entropy(int n, float *pred, float *truth, float *delta, float *error);


#endif //CUDA_NN_TRAIN_PROJ4_TRAINING_H
