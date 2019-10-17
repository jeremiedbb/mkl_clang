#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <pthread.h>


typedef void (*_dot)(double*, double*, double*, int, int, int);


void use_numpy_dot(_dot numpy_dot){
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


int main(void){
    printf("> from threadpoolctl import threadpool_info\n");
    void *threadpoolctl_handle = dlopen("/home/jeremie/R/Segfault/C/mkl_libomp_pthread/libthreadpoolctl.so", RTLD_NOW | RTLD_LOCAL);
    void *(*threadpool_info)(void*) = (void *(*)(void*)) dlsym(threadpoolctl_handle, "threadpool_info");
    
    printf("> threadpool_info()\n");
    *threadpool_info(NULL);

    // import numpy
    printf("> import numpy\n");
    void *numpy_handle = dlopen("/home/jeremie/R/Segfault/C/mkl_libomp_pthread/libnumpy.so", RTLD_NOW | RTLD_GLOBAL);

    printf("> threadpool_info()\n");
    *threadpool_info(NULL);

    // nested libomp loop / dgemm
    printf("> from nested_prange_blas import prange_blas\n");
    void *nested_handle = dlopen("/home/jeremie/R/Segfault/C/mkl_libomp_pthread/libnested.so", RTLD_NOW | RTLD_GLOBAL);
    void (*prange_blas)(int) = (void (*)(int)) dlsym(nested_handle, "prange_blas");
    
    printf("> prange_blas(4)\n");
    prange_blas(4);

    printf("> threadpool_info()\n");
    *threadpool_info(NULL);

    // numpy dot
    printf("> numpy.dot(A, B.T)\n");
    _dot numpy_dot = (_dot) dlsym(numpy_handle, "numpy_dot");
    use_numpy_dot(numpy_dot);

    // call threadpool info in a new thread
    printf("> t = threading.Thread(target=threadpool_info)\n");
    printf("> t.start()\n");
    pthread_t t;
    if(pthread_create(&t, NULL, threadpool_info, NULL)) {
        printf("Error creating thread\n");
        return 1;
    }
    printf("> t.join()\n");
    if(pthread_join(t, NULL)) {
        printf("Error joining thread\n");
        return 1;
    }

    return 0;
}
