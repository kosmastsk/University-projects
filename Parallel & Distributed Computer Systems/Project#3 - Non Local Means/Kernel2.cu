#include <math.h>
#include <stdio.h>

// Array access macros
#define b(i,j) B[(i) + (j)*m*m]

__global__ void Kernel2(double *I,double *B, int m, int n, int patchSize, double filtSigma) {
        // Get pixel (x,y) in input
        int i = blockIdx.x * blockDim.x + threadIdx.x;
        int j = blockIdx.y * blockDim.y + threadIdx.y;
        int pixelid = i+j*m;
        double sum = 0;

        if(pixelid<m*n) {
                int l=0;
                double sum2 = 0;

                for(int p = 0; p<pixelid; p++) {
                        sum2 = sum2 +  m*n-1 - p;
                }
                double sumImatrix = 0;
                double sumRows = 0;
                for(l=0; l<m*n; l++) {
                        sum = 0;
                        for(int k=0; k<patchSize*patchSize; k++) {
                                sum+=(b(pixelid,k)-b(l,k))*(b(pixelid,k)-b(l,k)); //calculate distances and add them to sum
                        }
                        sumRows +=  exp(-sqrt(sum)*sqrt(sum)/filtSigma);
                        sumImatrix += exp(-sqrt(sum)*sqrt(sum)/filtSigma)*I[l];
                }
                I[pixelid] = sumImatrix/sumRows;
        }
}
