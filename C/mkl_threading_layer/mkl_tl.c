#define _GNU_SOURCE
#include <link.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mkl.h"


int callback(struct dl_phdr_info *info, size_t size, void *data){
    if (strstr(info->dlpi_name, "/") != NULL) {
        printf("  - %s\n", info->dlpi_name);
    }
    return 0;
}


void printlibs(){
    printf("shared libraries:\n");
    dl_iterate_phdr(callback, NULL);
    printf("\n");
}


int main(void){
    printlibs();

    int m = 10000,
        n = 1000,
        k = 100;
    
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

    mkl_set_threading_layer(MKL_THREADING_TBB);

    printlibs();
    
    dgemm(trans, no_trans, &n, &m, &k, &alpha, B, &k, A, &k, &beta, C, &n);

    for(int i=0; i<m*n; i++){
        if(C[i] != (double)k ){
            printf("%f WRONG RESULT %i", C[i], i);
            break;
        }
    }

    printlibs();

    free(A), free(B), free(C);
}
