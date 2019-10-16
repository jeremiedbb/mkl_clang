#include <stdlib.h>
#include <dlfcn.h>


void use_mkl(void *handle){
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

  void (*dgemm)(char*, char*, int*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*) = (void (*)(char*, char*, int*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*)) dlsym(handle, "dgemm");
  dgemm(trans, no_trans, &n, &m, &k, &alpha, B, &k, A, &k, &beta, C, &n);

  free(A), free(B), free(C);
}


void use_omp_loop(void *handle){
    int n = 1000;

    double *A = (double*) malloc(n * sizeof(double)),
           *B = (double*) malloc(n * sizeof(double)),
           *C = (double*) malloc(n * sizeof(double));
    
    for(int i=0; i<n; i++){
        A[i] = i;
        B[i] = 2 * i;
    }

    void (*simple_loop)(double*, double*, double*, int) = (void (*)(double*, double*, double*, int)) dlsym(handle, "simple_loop");
    simple_loop(A, B, C, n);

    free(A), free(B), free(C);
}


int main(int argc, char **argv){
    // load mkl and omp_loop
    void *mkl_handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_rt.so", RTLD_LAZY | RTLD_GLOBAL);
    void *omp_loop_handle = dlopen("/home/jeremie/R/Segfault/C/libomp_loop.so", RTLD_LAZY | RTLD_LOCAL);

    // run a function from each one
    // switch order => segfault
    use_mkl(mkl_handle);
    use_omp_loop(omp_loop_handle);

    return 0;
}
