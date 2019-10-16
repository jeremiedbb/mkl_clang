#define _GNU_SOURCE
#include <link.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
}


int main(void){
    printlibs();

    void *mkl_handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_rt.so", RTLD_NOW | RTLD_GLOBAL);

    printlibs();

    use_mkl(mkl_handle);

    printlibs();

    void *handle;

    int i = 0;
    do {
        handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_vml_avx2.so", RTLD_LAZY | RTLD_NOLOAD);
        dlclose(handle);
    } while(handle && ++i < 100);
    if(i == 100){
        printf("Impossible to dlclose libmkl_vml_avx2\n\n");
    }

    handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_avx2.so", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);
    handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_intel_lp64.so", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);
    handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_intel_thread.so", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);
    handle = dlopen("/lib/x86_64-linux-gnu/libgcc_s.so.1", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);
    handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/../../../compiler/lib/intel64/libiomp5.so", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);
    handle = dlopen("/lib/x86_64-linux-gnu/libpthread.so.0", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);
    handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/libmkl_core.so", RTLD_NOW | RTLD_NOLOAD);
    dlclose(handle);

    dlclose(mkl_handle);
    
    printlibs();

    return 0;
}
