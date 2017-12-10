//
// Created by achilleas on 2/10/2017.
//

#ifndef CUDA_NN_TRAIN_PROJ4_ACTIVATIONS_H
#define CUDA_NN_TRAIN_PROJ4_ACTIVATIONS_H

#include "math.h"
#include "cuda_utils.h"

typedef enum{
    SIGMOID, RELU, TANH
} ACTIVATION;


void activate_array_gpu(float *x, int n, ACTIVATION a);
void gradient_array_gpu(float *x, int n, ACTIVATION a, float *delta);


#endif //CUDA_NN_TRAIN_PROJ4_ACTIVATIONS_H
