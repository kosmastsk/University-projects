#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "sys/time.h"
//#include "utils.h"
////#include "mnist-utils.h"
//#include "network.h"

#include "network.h"
//#include "mnist-utils.h"
#include "blas.h"


#define lr 0.02
#define batch 32

int main(int argc, const char * argv[]) {

    // open MNIST files
    FILE *imageFile, *labelFile;
    imageFile = openMNISTImageFile(MNIST_TRAINING_SET_IMAGE_FILE_NAME);
    labelFile = openMNISTLabelFile(MNIST_TRAINING_SET_LABEL_FILE_NAME);

    MNIST_Image *train_imgs;
    MNIST_Label *train_lbls;

    train_imgs = (MNIST_Image *)malloc(MNIST_MAX_TRAINING_IMAGES * sizeof(MNIST_Image));
    train_lbls = (MNIST_Label *)malloc(MNIST_MAX_TRAINING_IMAGES * sizeof(MNIST_Label));

    // Loop through all images in the file
    // and save them in a vector of images and labels
    //#pragma omp parallel for
    for (int imgCount=0; imgCount<MNIST_MAX_TRAINING_IMAGES; imgCount++) {

        // Reading next image and its corresponding label
        MNIST_Image img = getImage(imageFile);
        MNIST_Label lbl = getLabel(labelFile);

        train_imgs[imgCount] = img;
        train_lbls[imgCount] = lbl;
    }


    printf("\nLOADED ALL THE MNIST IMAGES\n\n");

    // Create neural network using a manually allocated memory space
    network net = create_network(2,MNIST_IMG_HEIGHT*MNIST_IMG_WIDTH,30,10,batch,lr);


    // Start training
    train_network(net,train_imgs,train_lbls,batch,MNIST_MAX_TRAINING_IMAGES);

    free_network(net);
    free(train_imgs);
    free(train_lbls);
    return 0;
}


