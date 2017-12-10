//
// Created by achilleas on 5/10/2017.
//

#include "cuda_utils.h"
#include <assert.h>
#include <math.h>
#include "blas.h"


void push_array_to_gpu(float *x_gpu, float *x, size_t n)
{
    size_t size = sizeof(float)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    assert(status==cudaSuccess);
}

void pull_array_from_gpu(float *x_gpu, float *x, size_t n)
{
    size_t size = sizeof(float)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    assert(status==cudaSuccess);
}

float *create_array_on_gpu(float *x, size_t n)
{
    float *x_gpu;
    size_t size = sizeof(float)*n;
    cudaError_t status = cudaMalloc((void **)&x_gpu, size);
    assert(status==cudaSuccess);
    if(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        assert(status==cudaSuccess);
    } else {
        zero_init_gpu_array(n, x_gpu);
    }
    if(!x_gpu) printf("Cuda malloc failed\n");
    return x_gpu;
}

void cuda_free(float *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
}


// manually
dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    if(x > 65535){ //compute power <1
        x = ceil(sqrt(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    return d;
}

