CC          = clang-8
LINKER      = $(CC)

all:
	$(CC) -fPIC -c -o threadpoolctl.o threadpoolctl.c
	$(LINKER) -shared -o libthreadpoolctl.so threadpoolctl.o -ldl

	$(CC) -fPIC -c -o numpy.o numpy.c
	$(LINKER) -shared -o libnumpy.so numpy.o -lmkl_rt

	$(CC) -fopenmp -fPIC -c -o nested.o nested.c
	$(LINKER) -shared -fopenmp -o libnested.so nested.o -lmkl_rt

	$(CC) -o test main.c -ldl -lpthread
	KMP_DUPLICATE_LIB_OK=TRUE ./test

clean:
	rm -f *.o *.so
	rm -f test
