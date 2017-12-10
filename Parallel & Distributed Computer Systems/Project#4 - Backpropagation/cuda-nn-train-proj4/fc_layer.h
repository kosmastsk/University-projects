//
// Created by achilleas on 2/10/2017.
//

#ifndef CUDA_NN_TRAIN_PROJ4_FC_LAYER_H
#define CUDA_NN_TRAIN_PROJ4_FC_LAYER_H

#include "activations.h"
#include "network.h"

typedef struct fc_layer fc_layer;

struct fc_layer{
    ACTIVATION activation;

    int isOutput;

    int batch;
    int inputs;
    int outputs;
    int truths;
    float *cost;


    float * biases;
    float * bias_updates;

    float * weights;
    float * weight_updates;

    float * delta;
    float * output;

    float * weights_gpu;
    float * weight_updates_gpu;

    float * biases_gpu;
    float * bias_updates_gpu;

    float * output_gpu;
    float * delta_gpu;

};

fc_layer create_fc_layer(int batch, int inputs, int outputs, ACTIVATION activation, int isOutput);
void free_layer(fc_layer);

void forward_pass_fc_layer(fc_layer l, network net);
void backward_pass_fc_layer(fc_layer l, network net);
void update_fc_layer(fc_layer l, network net);

void calc_error(fc_layer l, network net);
void calc_delta(const fc_layer l, network net);
float sum_array(float *a, int n);


#endif //CUDA_NN_TRAIN_PROJ4_FC_LAYER_H

