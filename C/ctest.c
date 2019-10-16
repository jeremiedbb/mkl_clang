#define _GNU_SOURCE
#include <link.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>


int callback(struct dl_phdr_info *info, size_t size, void *data){
    if (strstr(info->dlpi_name, "/") != NULL) {
        printf("  - %s\n", info->dlpi_name);
    }
    return 0;
}


int main(int argc, char **argv){
	// shared_lib_handle = dlopen("libcmodule.so", RTLD_NOW | RTLD_GLOBAL);
	// prange_blas = (void (*)(int)) dlsym(shared_lib_handle, "prange_blas");
	// mkl_get_max_threads = (int (*)()) dlsym(shared_lib_handle, "mkl_get_max_threads_");
	// // omp_get_max_threads_1 = (int (*)()) dlsym(shared_lib_handle, "omp_get_max_threads_");

    // prange_blas(4);

    void *mkl_only_handle = dlopen("libmkl_only.so", RTLD_NOW | RTLD_GLOBAL);
	void (*matmul)() = (void (*)(int)) dlsym(mkl_only_handle, "matmul");
    // matmul();

    // void *cmodule_handle = dlopen("libcmodule.so", RTLD_NOW | RTLD_LOCAL);
    // void (*prange_blas)() = (void (*)(int)) dlsym(cmodule_handle, "prange_blas");
    // prange_blas();

    printf("shared libraries:\n");
    dl_iterate_phdr(callback, NULL);
    printf("\n");


    // printf("shared libraries:\n");
    // dl_iterate_phdr(callback, NULL);
    // printf("\n");


    void *libomp_handle = dlopen("/usr/lib/x86_64-linux-gnu/libomp.so.5", RTLD_NOW | RTLD_LOCAL);
    printf("libomp handle = %p\n", libomp_handle);
    void *libiomp_handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64_lin/../../../compiler/lib/intel64/libiomp5.so", RTLD_NOW | RTLD_LOCAL);
    printf("libiomp handle = %p\n", libiomp_handle);
    
    int (*libomp_get_max_threads)() = (int (*)()) dlsym(libomp_handle, "omp_get_max_threads");
    printf("libomp %i\n", libomp_get_max_threads());
    int (*libiomp_get_max_threads)() = (int (*)()) dlsym(libiomp_handle, "omp_get_max_threads");
    printf("libiomp %i\n", libiomp_get_max_threads());
    // int (*libiomp_get_max_threads_2)() = (int (*)()) dlsym(libiomp_handle, "omp_get_max_threads");
    // printf("libiomp %i\n", libiomp_get_max_threads_2());



    // Dl_info dlInfo;
    // if(!dladdr(matmul, &dlInfo)) {
    // // dladdr() failed.
    // }
    // printf("%s\n", dlInfo.dli_fname);


    // int (*omp_get_max_threads_1)() = (int (*)()) dlsym(RTLD_DEFAULT, "omp_get_max_threads");
    // if(!dladdr(omp_get_max_threads_1, &dlInfo)) {
    // // dladdr() failed.
    // }
    // printf("%s\n", dlInfo.dli_fname);

    // printf("%i, %i", RTLD_DEFAULT, RTLD_NEXT);

    // // printf("omp = %i\n", omp_get_max_threads_1());

    // omp_get_max_threads_2 = (int (*)()) dlsym(RTLD_NEXT, "omp_get_max_threads");
    // // printf("omp = %i\n", omp_get_max_threads_1());

    // void *libiomp_handle;
    // libiomp_handle = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/compiler/lib/intel64/libiomp5.so", RTLD_NOW | RTLD_NOLOAD);
    // if(!libiomp_handle){
    //     printf("LOOOOOOOOOOOOOOOOOL");
    // }
    
    // void *libiomp_handle_2 = dlopen("/home/jeremie/intel/compilers_and_libraries_2019.4.243/linux/compiler/lib/intel64/libiomp5.so", RTLD_NOW | RTLD_LOCAL);

    // printf("%p\n", libiomp_handle);
    // printf("%p\n", libiomp_handle_2);
    
    // int (*iomp_get_max_threads)();
    // iomp_get_max_threads = (int (*)()) dlsym(libiomp_handle, "omp_get_max_threads");
    // int (*iomp_get_max_threads_2)();
    // iomp_get_max_threads_2 = (int (*)()) dlsym(libiomp_handle_2, "omp_get_max_threads");

    // printf("mkl = %i\n", mkl_get_max_threads());
    // printf("omp = %i\n", omp_get_max_threads());
    // printf("iomp = %i\n", iomp_get_max_threads());
    // printf("iomp2 = %i\n", iomp_get_max_threads_2());

    return 0;
}
