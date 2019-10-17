#include <stdlib.h>
#include <stdio.h>
#include "mkl.h"


void numpy_dot(double *A, double *B, double *C, int m, int n, int k){
    char *trans = "t",
         *no_trans = "n";
    double alpha = 1.0,
           beta = 0.0;

    dgemm(trans, no_trans, &n, &m, &k, &alpha, B, &k, A, &k, &beta, C, &n);
}


void __attribute ((constructor)) init_function(){
    int m = 10000,
        n = 1000,
        k = 100;
    
    double *A = (double*) malloc(m * k * sizeof(double)),
           *B = (double*) malloc(n * k * sizeof(double)),
           *C = (double*) malloc(m * n * sizeof(double));

    for(int i=0; i<m*k; i++){
        A[i] = 1.0;
    }
    for(int i=0; i<n*k; i++){
        B[i] = 1.0;
    }

    numpy_dot(A, B, C, m, n, k);

    free(A), free(B), free(C);
}
