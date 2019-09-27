CC          = clang-8
CFLAGS      = -Wall -Werror
LINKER      = $(CC)

nothing:

cython: build_cython test_cython

build_cython:
	@echo "Build\n"
	CC=$(CC) python setup.py build_ext -i

test_cython:
	@echo "\n\nTest\n"
	python -c 'from prange_blas import prange_blas; prange_blas(4)'
	#python -c 'from prange_blas import prange_blas; from threadpoolctl import threadpool_info; threadpool_info(); prange_blas(4)'

c: build_c test_c
	
build_c:
	@echo "Build\n"
	$(CC) $(CFLAGS) -fPIC -c -fopenmp -o shared_lib.o shared_lib.c
	$(LINKER) -shared -fopenmp -o libshared_lib.so shared_lib.o -lmkl_rt
	$(CC) $(CFLAGS) -I. -c main.c -o main.o
	$(LINKER) -L. -Wl,-rpath=. main.o -o run -lshared_lib

test_c:
	@echo "\n\nTest\n"
	./run

clean:
	rm -f *.so *.o run
	rm prange_blas.c
	rm -rf build
