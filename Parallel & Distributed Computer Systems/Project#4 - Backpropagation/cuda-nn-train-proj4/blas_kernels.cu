#include "cuda_runtime.h"
#include "cublas_v2.h"
#include <assert.h>
#include <math.h>
#include "blas.h"
#include "cuda_utils.h"



__global__ void launch_axpy_kernel(int N, float ALPHA, float *X, int INCX,  float *Y, int INCY)
{
    int i = (blockIdx.x + blockIdx.y*gridDim.x) * blockDim.x + threadIdx.x;
    if(i < N) Y[i*INCY] += ALPHA*X[i*INCX];
}

__global__ void launch_zero_init_kernel(int N, float *X)
{
    int i = (blockIdx.x + blockIdx.y*gridDim.x) * blockDim.x + threadIdx.x;
    if(i < N) X[i] = 0.f;
}

__global__ void launch_cross_entropy_kernel(int n, float *pred, float *truth, float *delta, float *error)
{
    int i = (blockIdx.x + blockIdx.y*gridDim.x) * blockDim.x + threadIdx.x;
    if(i < n){
        float diff = truth[i] - pred[i];
        error[i] = -truth[i]*log(pred[i]) - (1-truth[i])*log(1-pred[i]);
        delta[i] = diff;
    }
}

extern "C" void zero_init_gpu_array(int N, float * X)
{
    launch_zero_init_kernel<<<cuda_gridsize(N), BLOCK>>>(N, X);
}

extern "C" void axpy_cuda(int N, float ALPHA, float * X, int INCX, float * Y, int INCY)
{
    launch_axpy_kernel<<<cuda_gridsize(N), BLOCK>>>(N, ALPHA, X, INCX, Y, INCY);
}


extern "C" void cross_entropy(int n, float *pred, float *truth, float *delta, float *error)
{
    launch_cross_entropy_kernel<<<cuda_gridsize(n), BLOCK>>>(n, pred, truth, delta, error);
}