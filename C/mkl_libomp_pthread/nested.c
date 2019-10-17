#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "mkl.h"


void prange_blas(int nthreads){
    int m = 10000,
        n = 1000,
        k = 100;
    
    int n_chunks = 100,
        chunk_size = m / n_chunks;

    char *trans = "t",
         *no_trans = "n";
    double alpha = 1.0,
           beta = 0.0;

    double *A = (double*) malloc(m * k * sizeof(double)),
           *B = (double*) malloc(n * k * sizeof(double)),
           *C = (double*) malloc(m * n * sizeof(double));

    for(int i=0; i<m*k; i++){
        A[i] = 1.0;
    }
    for(int i=0; i<n*k; i++){
        B[i] = 1.0;
    }
    
    #pragma omp parallel num_threads(nthreads)
    {
        printf("  - thread num = %i\n", omp_get_thread_num());

        #pragma omp for
        for(int i=0; i<n_chunks; i++){
            dgemm(trans, no_trans, &n, &chunk_size, &k,
                  &alpha, B, &k, A + i * k * chunk_size, &k,
                  &beta, C + i * n * chunk_size, &n);
        }
    }

    for(int i=0; i<m*n; i++){
        if(C[i] != (double)k ){
            printf("%f WRONG RESULT %i", C[i], i);
            break;
        }
    }

    free(A), free(B), free(C);
}
