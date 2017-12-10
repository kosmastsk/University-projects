//
// Created by achilleas on 2/10/2017.
//

#ifndef CUDA_NN_TRAIN_PROJ4_NETWORK_H
#define CUDA_NN_TRAIN_PROJ4_NETWORK_H

#include "mnist-utils.h"
#include "fc_layer.h"

typedef struct network{
    int n;
    int batch;
    float epoch;
    fc_layer *layers;
    float *output;

    double learning_rate;


    int inputs;
    int outputs;
    int truths;

    float *input;
    float *truth;
    float *delta;
    int train;
    int index;
    float *cost;

    float *input_gpu;
    float *truth_gpu;
    float *delta_gpu;
    float *output_gpu;

} network;

float train_network_batch_gpu(network net, MNIST_Image *train_images, MNIST_Label *train_labels, int batch_size);
void train_network(network net, MNIST_Image *train_images, MNIST_Label *train_labels, int batch_size, int total_size);

network init_network(int n);
network create_network(int n, int num_inputs, int num_hidden, int num_output, int batch_size, double learning_rate);

void update_network_gpu(network net);
void backward_network_gpu(network net);
void forward_network_gpu(network net);

void free_network(network net);

#endif //CUDA_NN_TRAIN_PROJ4_NETWORK_H
