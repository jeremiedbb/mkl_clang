#include <dlfcn.h>

int main(int argc, char **argv){
    void *shared_lib_handle;
    void (*f)(int);

	shared_lib_handle = dlopen("libcmodule.so", RTLD_NOW | RTLD_GLOBAL);
	f = (void (*)(int)) dlsym(shared_lib_handle, "prange_blas");

    f(4);

    return 0;
}
