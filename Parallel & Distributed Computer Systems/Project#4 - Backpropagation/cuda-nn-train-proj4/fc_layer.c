//
// Created by achilleas on 2/10/2017.
//

#include "fc_layer.h"
#include "blas.h"
#include "cuda_utils.h"



float rand_uniform(float min, float max)
{
    if(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    return ((float)rand()/RAND_MAX * (max - min)) + min;
}

fc_layer create_fc_layer(int batch, int inputs, int outputs, ACTIVATION activation, int isOutput)
{
    int i;
    fc_layer l = {0};

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.activation = activation;
    l.isOutput = isOutput;

    l.output = calloc(batch*outputs, sizeof(float));
    l.delta = calloc(batch*outputs, sizeof(float));

    if(l.isOutput){
        l.cost = calloc(1, sizeof(float));
    }

    l.weight_updates = calloc(inputs*outputs, sizeof(float));
    l.bias_updates = calloc(outputs, sizeof(float));

    l.weights = calloc(outputs*inputs, sizeof(float));
    l.biases = calloc(outputs, sizeof(float));

    //float scale = 1./sqrt(inputs);
    // weight init Xavier
    float scale = sqrt(2./inputs);

    //openmp?
    for(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    for(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    l.weights_gpu = create_array_on_gpu(l.weights, outputs * inputs);
    l.biases_gpu = create_array_on_gpu(l.biases, outputs);

    l.weight_updates_gpu = create_array_on_gpu(l.weight_updates, outputs * inputs);
    l.bias_updates_gpu = create_array_on_gpu(l.bias_updates, outputs);

    l.output_gpu = create_array_on_gpu(l.output, outputs * batch);
    l.delta_gpu = create_array_on_gpu(l.delta, outputs * batch);
    fprintf(stderr, "Fully Connected layer, connected %4d  ->  %4d\n", inputs, outputs);
    return l;
}


void forward_pass_fc_layer(fc_layer l, network net)
{
    zero_init_gpu_array(l.outputs * l.batch, l.output_gpu); // clear output

    //our matmul here
    int m = l.batch;
    int k = l.inputs;
    int n = l.outputs;
    float * a = net.input_gpu;
    float * b = l.weights_gpu;
    float * c = l.output_gpu;
    /*
     * cuBlas assumes column-major, c is row-major. So to pass the arrays correctly and actually compute Z=W*A
     * we need to have the correct order. So we use the identity: (AB)T = (B.T)(A.T), because if we read a row-major
     * array as column-major, we actually have the transpose.
     * */
    cuBlas_matmul(0, 1, m, n, k, 1, a, k, b, k, 1, c, n); // cuBlas_matmul does cublas  C = A*B+C, here Z = W*A
    // sugkrisi me diko mas matmul kernel

    int i;
    // add the biases
    for(i = 0; i < l.batch; ++i){
        axpy_cuda(l.outputs, 1, l.biases_gpu, 1, l.output_gpu + i * l.outputs, 1);
    }
    // Activation
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation); // Z = σ(Z) activation layer
}


void backward_pass_fc_layer(fc_layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu); //delta = prime(z) back to front
    zero_init_gpu_array(l.outputs * l.inputs, l.weight_updates_gpu); // Clear previous update, we don't use momentum

    int i;
    //biases backprop
    for(i = 0; i < l.batch; ++i){
        axpy_cuda(l.outputs, 1, l.delta_gpu + i * l.outputs, 1, l.bias_updates_gpu, 1);
    }

    // l.delta_gpu has prime(z)
    // to net.delta_gpu exei to delta_gpu tou proigoumenou layer
    int m = l.outputs;
    int k = l.batch;
    int n = l.inputs;
    float * a = l.delta_gpu;
    float * b = net.input_gpu;
    float * c = l.weight_updates_gpu;
    // C = A*B+C , but C is always zero before, since we  clear it at each update (no Momentum)
    cuBlas_matmul(1, 0, m, n, k, 1, a, m, b, n, 1, c, n);

    // weight_updates = delta_gpu * net.input_gpu (activations)
    //Update_weights = delta*activation[-1]

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    if(c) cuBlas_matmul(0, 0, m, n, k, 1, a, k, b, n, 1, c, n);
    // a is prime(z) of curr layer, b weights of curr layer
    // c is delta of previous layer. Again the deltas have  been cleared in the forward pass, so its C=A*B+0.
    // delta = weightsT * delta(l-1) * prime
}


void update_fc_layer(fc_layer l, network net)
{
    double learning_rate = net.learning_rate;
    int batch = net.batch;

    //no momentum
    //bias update
    axpy_cuda(l.outputs, learning_rate / batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    //weight update
    axpy_cuda(l.inputs * l.outputs, learning_rate / batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
}


void calc_error(fc_layer l, network net)
{
    if (!net.truth_gpu) return;

    cross_entropy(l.batch*l.outputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu); // to diko mas edw

    pull_array_from_gpu(l.output_gpu, l.output, l.batch * l.outputs);
    l.cost[0] = sum_array(l.output, l.batch*l.outputs);
}


float sum_array(float *a, int n)
{
    int i;
    float sum = 0;
    for(i = 0; i < n; ++i) sum += a[i];
    return sum;
}

void free_layer(fc_layer l)
{

    if(l.cost)               free(l.cost);
    if(l.biases)             free(l.biases);
    if(l.bias_updates)       free(l.bias_updates);
    if(l.weights)            free(l.weights);
    if(l.weight_updates)     free(l.weight_updates);
    if(l.delta)              free(l.delta);
    if(l.output)             free(l.output);
    if(l.weights_gpu)             cuda_free(l.weights_gpu);
    if(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    if(l.biases_gpu)              cuda_free(l.biases_gpu);
    if(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    if(l.output_gpu)              cuda_free(l.output_gpu);
    if(l.delta_gpu)               cuda_free(l.delta_gpu);

}


//
//void calc_delta(const fc_layer l, network net)
//{
//    axpy_cuda(l.batch * l.outputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
//}
