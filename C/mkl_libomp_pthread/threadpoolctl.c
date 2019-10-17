#define _GNU_SOURCE
#include <link.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>


static int callback(struct dl_phdr_info *info, size_t size, void *data){
    if (strstr(info->dlpi_name, "/") != NULL){
        void *handle = dlopen(info->dlpi_name, RTLD_NOW | RTLD_LOCAL);
        if (strstr(basename(info->dlpi_name), "libmkl_rt") != NULL) {
            int (*get)() = (int (*)()) dlsym(handle, "mkl_get_max_threads");
            printf("  - %s\n", basename(info->dlpi_name));
            printf("    max threads = %i\n", get());
        }
        else if (strstr(basename(info->dlpi_name), "libomp") != NULL ||
                 strstr(basename(info->dlpi_name), "libiomp") != NULL ||
                 strstr(basename(info->dlpi_name), "libgomp") != NULL) {
            int (*get)() = (int (*)()) dlsym(handle, "omp_get_max_threads");
            printf("  - %s\n", basename(info->dlpi_name));
            printf("    max threads = %i\n", get());
        }
    }
    return 0;
}


void *threadpool_info(void *unused){
    dl_iterate_phdr(callback, NULL);

    return NULL;
}