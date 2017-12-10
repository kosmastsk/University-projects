//
// Created by achilleas on 5/10/2017.
//

#ifndef CUDA_NN_TRAIN_PROJ4_CUDA_UTILS_H
#define CUDA_NN_TRAIN_PROJ4_CUDA_UTILS_H

#define BLOCK 512

#include "cuda_runtime.h"
#include "curand.h"
#include "cublas_v2.h"
#include <stdlib.h>
#include <stdio.h>

dim3 cuda_gridsize(size_t n);
void cuda_free(float *x_gpu);
float *create_array_on_gpu(float *x, size_t n);
void pull_array_from_gpu(float *x_gpu, float *x, size_t n);
void push_array_to_gpu(float *x_gpu, float *x, size_t n);


#endif //CUDA_NN_TRAIN_PROJ4_CUDA_UTILS_H
