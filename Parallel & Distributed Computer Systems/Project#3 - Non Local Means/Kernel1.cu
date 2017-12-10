#include <math.h>
#include <stdio.h>

// Array access macros
#define INPUT(i,j) A[(i) + (j)*(m+patchSize-1)]
#define OUTPUT(i,j) B[(i) + (j)*m*m]
#define FILTER(i) H[(i)]

__global__ void sampleAdd(double const * const A, double *B, double *H, int m, int n, int patchSize) {
        // Get pixel (x,y) in input
        int i = blockIdx.x * blockDim.x + threadIdx.x;
        int j = blockIdx.y * blockDim.y + threadIdx.y;

        if (i>=((patchSize - 1) / 2) && i<=m+((patchSize - 1) / 2) && j>=((patchSize - 1) / 2) && j<=m+((patchSize - 1) / 2)) { //do not scan pixels in the pad
                for (int k = -(patchSize - 1) / 2; k <= (patchSize - 1) / 2; k++) { //scan the neighbours in an area patchSize x patchSize
                        for (int l = -(patchSize - 1) / 2; l <= (patchSize - 1) / 2; l++) {
                                OUTPUT(i - ((patchSize - 1) / 2) + m * (j - ((patchSize - 1) / 2)), k + ((patchSize - 1) / 2) + (l + ((patchSize - 1) / 2)) * patchSize) = INPUT(k + i, l + j); //assign the neighbors' value
                                OUTPUT(i - ((patchSize - 1) / 2) + m * (j - ((patchSize - 1) / 2)), k + ((patchSize - 1) / 2) + (l + ((patchSize - 1) / 2)) * patchSize) *= (FILTER(k + ((patchSize - 1) / 2) + (l + ((patchSize - 1) / 2)) * patchSize)); //multiply that value with a filter
                        }
                }
        }
}
