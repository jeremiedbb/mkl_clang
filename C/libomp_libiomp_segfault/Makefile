all: simple advanced

simple:
	gcc -Wall -o simple_omp simple_omp.c -ldl 
	KMP_DUPLICATE_LIB_OK=TRUE ./simple_omp

advanced:
	clang-8 -Wall -fopenmp -fPIC -c -o omp_loop.o omp_loop.c
	clang-8 -shared -fopenmp -o libomp_loop.so omp_loop.o
	gcc -Wall -o advanced_omp advanced_omp.c -ldl 
	KMP_DUPLICATE_LIB_OK=TRUE ./advanced_omp

clean:
	rm -f *.so *.o
	rm -f advanced_omp simple_omp