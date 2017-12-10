#include "cuda_runtime.h"
#include "activations.h"
#include "cuda_utils.h"


__device__ float sigmoid_activate_kernel(float x){return 1./(1. + exp(-x));}
__device__ float relu_activate_kernel(float x){return x*(x>0);}
__device__ float tanh_activate_kernel(float x){return (2/(1 + exp(-2*x)) - 1);}

__device__ float sigmoid_gradient_kernel(float x){return (1-x)*x;}
__device__ float relu_gradient_kernel(float x){return (x>0);}
__device__ float tanh_gradient_kernel(float x){return 1-x*x;}


__device__ float activate_kernel(float x, ACTIVATION a)
{
    switch(a){
        case SIGMOID:
            return sigmoid_activate_kernel(x);
        case RELU:
            return relu_activate_kernel(x);
        case TANH:
            return tanh_activate_kernel(x);
    }
    return 0;
}

__device__ float gradient_kernel(float x, ACTIVATION a)
{
    switch(a){
        case SIGMOID:
            return sigmoid_gradient_kernel(x);
        case RELU:
            return relu_gradient_kernel(x);
        case TANH:
            return tanh_gradient_kernel(x);
    }
    return 0;
}

// sti cuda utils 8a mpei to check error kai to cudagridsize kai to blocksize, heuristic grid_size, todo  cudaOccupancy

__global__ void activate_array_kernel(float *x, int n, ACTIVATION a)
{
    int i = (blockIdx.x + blockIdx.y*gridDim.x) * blockDim.x + threadIdx.x; // check stackoverflow
    // https://stackoverflow.com/questions/16403972/cuda-thread-addressing-threadidx-x-threadidx-y-threadidx-z-and-block-addres?rq=1
    // na ginei me for?
    if(i < n) x[i] = activate_kernel(x[i], a);
}

__global__ void gradient_array_kernel(float *x, int n, ACTIVATION a, float *delta)
{
    int i = (blockIdx.x + blockIdx.y*gridDim.x) * blockDim.x + threadIdx.x;
    if(i < n) delta[i] *= gradient_kernel(x[i], a);
}

extern "C" void activate_array_gpu(float *x, int n, ACTIVATION a)
{
    activate_array_kernel<<<cuda_gridsize(n), BLOCK>>>(x, n, a);
}

extern "C" void gradient_array_gpu(float *x, int n, ACTIVATION a, float *delta) {
    gradient_array_kernel << < cuda_gridsize(n), BLOCK >> > (x, n, a, delta);
}