#include "network.h"
#include "blas.h"
#include "fc_layer.h"
#include "mnist-utils.h"


void forward_network_gpu(network net){
    int i;
    for (i=0;i<net.n;i++){
        net.index = i;

        fc_layer l = net.layers[i];

        if(l.delta_gpu){
            zero_init_gpu_array(l.outputs * l.batch, l.delta_gpu); //clear the delta from the previous pass
        }

        if(l.isOutput){
            forward_pass_fc_layer(l, net);
            calc_error(l, net);
            *net.cost = l.cost[0];

        }else{
            forward_pass_fc_layer(l, net);
        }

        net.input_gpu = l.output_gpu;
        net.input = l.output;
        if(l.isOutput) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }

}


void backward_network_gpu(network net){
    int i;
    network original = net;
    for (i=net.n-1; i>=0; i--){
        net.index = i; // where are we in the net
        fc_layer l = net.layers[i];
        if(i==0){
            net = original; //go back to initial state
        }else{
            fc_layer prev_layer = net.layers[i-1];
            net.input = prev_layer.output;
            net.delta = prev_layer.delta;
            net.input_gpu = prev_layer.output_gpu;
            net.delta_gpu = prev_layer.delta_gpu;
        }

        backward_pass_fc_layer(l, net);

    }

}
void update_network_gpu(network net){

    int i;
    for (i=0; i<net.n; i++){
        fc_layer l = net.layers[i];
        update_fc_layer(l, net);
    }

}

network create_network(int n, int num_inputs, int num_hidden, int num_output, int batch_size, double learning_rate){
    network net = init_network(n);
    ACTIVATION act = SIGMOID;

    fc_layer l1 = create_fc_layer(batch_size, num_inputs, num_hidden, act, 0);
    fc_layer l2 = create_fc_layer(batch_size, num_hidden, num_output, act, 1);
    // cost layer me inputs = num_output
    net.layers[0] = l1;
    net.layers[1] = l2;

    net.learning_rate = learning_rate;
    net.batch = batch_size;

    net.outputs = l2.outputs;
    net.truths = l2.outputs;

    net.output = l2.output;
    net.input = calloc(net.inputs*net.batch, sizeof(float));
    net.truth = calloc(net.outputs*net.batch, sizeof(float));

    net.output_gpu = l2.output_gpu;
    net.input_gpu = create_array_on_gpu(net.input, net.inputs * net.batch);
    net.truth_gpu = create_array_on_gpu(net.truth, net.truths * net.batch);

    return net;
}


network init_network(int n)
{
    network net = {0};
    net.n = n;
    net.layers = calloc(net.n, sizeof(fc_layer));
    net.cost = calloc(1,sizeof(int));
    return net;
}

void free_network(network net)
{
    int i;
    for(i = 0; i < net.n; ++i){
        free_layer(net.layers[i]);
    }
    free(net.layers);
    if(net.input) free(net.input);
    if(net.truth) free(net.truth);
    if(net.input_gpu) cuda_free(net.input_gpu);
    if(net.truth_gpu) cuda_free(net.truth_gpu);

}


void train_network(network net, MNIST_Image *train_images, MNIST_Label *train_labels, int batch_size, int total_size){
    //todo epochs
    for (int i = 0, percent = 0; i <= total_size - batch_size; i += batch_size)
    {
        float loss = train_network_batch_gpu(net, train_images + i, train_labels + i, batch_size);
        if (i * 100 / total_size > percent)
            printf("batchsize:%d\ttrain:%2d%%\n", batch_size, percent = i * 100 / total_size);
    }
}

float train_network_batch_gpu(network net, MNIST_Image *train_images, MNIST_Label *train_labels, int batch_size){

    int input_size = net.inputs*net.batch;
    int output_size = net.outputs*net.batch;

    //must be made faster! cant make it work with memcpy! :(
    int i,j;
    for (i=0;i<batch_size;i++){
        for (j=0;j<net.inputs;j++){
            net.input[i*net.inputs+j]=train_images[i].pixel[j];
            //labels one-hot vector
        }
    }
    push_array_to_gpu(net.input_gpu, net.input, input_size);
    push_array_to_gpu(net.truth_gpu, net.truth, output_size);

    net.train = 1; // iniitiate training
    forward_network_gpu(net);
    backward_network_gpu(net);

    float error = *net.cost;
    update_network_gpu(net);

    return error/batch_size;

}



