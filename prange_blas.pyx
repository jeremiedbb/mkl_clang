cimport openmp
from cython.parallel import parallel, prange
from libc.stdlib cimport malloc, free
from libc.stdio cimport printf

#from scipy.linalg.cython_blas cimport dgemm

cdef extern from "mkl_blas.h":
    void dgemm 'dgemm' (
        const char *transa, const char *transb, const int *m, const int *n, const int *k,
        const double *alpha, const double *a, const int *lda, const double *b, const int *ldb,
        const double *beta, double *c, const int *ldc) nogil


def prange_blas(int nthreads):
    cdef:
        int m = 1000
        int n = 100
        int k = 10

        int n_chunks = 100
        int chunk_size = m // n_chunks

        char* trans = 't'
        char* no_trans = 'n'
        double alpha = 1.0
        double beta = 0.0

        int i

        double *A = <double*> malloc(m * k * sizeof(double))
        double *B = <double*> malloc(n * k * sizeof(double))
        double *C = <double*> malloc(m * n * sizeof(double))

    for i in range(m * k):
        A[i] = 1.0
    
    for i in range(n * k):
        B[i] = 1.0

    with nogil, parallel(num_threads=nthreads):
        printf("omp num threads in parallel region = %i\n", openmp.omp_get_num_threads())

        for i in prange(n_chunks):
            dgemm(trans, no_trans, &n, &chunk_size, &k,
                  &alpha, B, &k, A + i * k * chunk_size, &k,
                  &beta, C + i * n * chunk_size, &n)

    for i in range(m * n):
        if C[i] != k:
            print("{} WRONG RESULT {}".format(C[i], i))
            break
    
    free(A)
    free(B)
    free(C)


def only_blas():
    cdef:
        int m = 1000
        int n = 100
        int k = 10

        char* trans = 't'
        char* no_trans = 'n'
        double alpha = 1.0
        double beta = 0.0

        int i

        double *A = <double*> malloc(m * k * sizeof(double))
        double *B = <double*> malloc(n * k * sizeof(double))
        double *C = <double*> malloc(m * n * sizeof(double))

    for i in range(m * k):
        A[i] = 1.0
    
    for i in range(n):
        B[i] = 1.0

    dgemm(trans, no_trans, &n, &m, &k,
          &alpha, B, &k, A, &k,
          &beta, C, &n)
        
    free(A)
    free(B)
    free(C)


def only_prange(int nthreads):
    cdef:
        int m = 1000
        int k = 10

        int i

        double *A = <double*> malloc(m * k * sizeof(double))
        double *B = <double*> malloc(m * k * sizeof(double))

    for i in range(m * k):
        A[i] = 1.0
    
    for i in prange(m * k, nogil=True, num_threads=nthreads):
        B[i] = A[i]
    
    free(A)
    free(B)
