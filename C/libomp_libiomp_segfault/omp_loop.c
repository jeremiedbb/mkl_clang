#include <stdlib.h>
#include <omp.h>


void simple_loop(double *A, double *B, double *C, int n){
    #pragma omp parallel for
    {
        for(int i=0; i<n; i++){
            C[i] = A[i] + B[i];
        }
    }
}
