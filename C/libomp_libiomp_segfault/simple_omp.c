#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>


int main(int argc, char **argv){
    // load libomp and libiomp
    void *libomp_handle = dlopen("/home/jeremie/R/C/llvm-project/build/lib/libomp.so", RTLD_NOW | RTLD_LOCAL);
    if(!libomp_handle){
        printf("libomp not loaded\n");
    }
    void *libiomp_handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/../../../compiler/lib/intel64/libiomp5.so", RTLD_NOW | RTLD_GLOBAL);
    if(!libiomp_handle){
        printf("libiomp not loaded\n");
    }
    printf("%p  |  %p\n", libomp_handle, libiomp_handle);

    // get "omp_get_max_threads" from each library
    int (*libomp_get_max_threads)() = (int (*)()) dlsym(libomp_handle, "omp_get_max_threads");
    if(!libomp_get_max_threads){
        printf("failed finding libomp omp_get_max_threads\n");
    }
    int (*libiomp_get_max_threads)() = (int (*)()) dlsym(libiomp_handle, "omp_get_max_threads");
    if(!libiomp_get_max_threads){
        printf("failed finding libiomp omp_get_max_threads\n");
    }
    printf("%p  |  %p", libomp_get_max_threads, libiomp_get_max_threads);

    // switch order of calls causes SIGABRT
    libiomp_get_max_threads();
    libomp_get_max_threads();

    return 0;
}
