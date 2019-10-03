#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <omp.h>
#include <mkl_blas.h>


static PyObject * prange_blas(PyObject*, PyObject*);


static PyMethodDef cpmethods[] = {
    {"prange_blas", prange_blas, METH_VARARGS, "no docstring"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef cpmodule = {
    PyModuleDef_HEAD_INIT,
    "cpmodule", /* name of module */
    NULL,       /* module documentation, may be NULL */
    -1,         /* size of per-interpreter state of the module,
                   or -1 if the module keeps state in global variables. */
    cpmethods
};


PyMODINIT_FUNC PyInit_cpmodule(void){
    return PyModule_Create(&cpmodule);
}


static PyObject *prange_blas(PyObject *self, PyObject *args){
    const int nthreads;
    if (!PyArg_ParseTuple(args, "i", &nthreads))
        return NULL;
    
    int m = 1000;
    int n = 100;
    int k = 10;
    int n_chunks = 100;
    int chunk_size = m / n_chunks;

    char *trans = "t";
    char *no_trans = "n";
    double alpha = 1.0;
    double beta = 0.0;

    double *A = (double *) malloc(m * k * sizeof(double));
    double *B = (double *) malloc(n * k * sizeof(double));
    double *C = (double *) malloc(m * n * sizeof(double));

    for (int i = 0; i < m * k; i++) {
        (A[i]) = 1.0;
    }

    for (int i = 0; i < n * k; i++) {
        (B[i]) = 1.0;
    }

    #pragma omp parallel num_threads(nthreads)
    {
        printf("omp num threads in parallel region = %i\n", omp_get_num_threads());
        
        #pragma omp for
        for (int i = 0; i < n_chunks; i++){
            dgemm(trans, no_trans, &n, &chunk_size, &k,
                    &alpha, B, &k, A + i * k * chunk_size, &k,
                    &beta, C + i * n * chunk_size, &n);
        }
    }

    for (int i = 0; i < m * n; i++) {
        if (C[i] != k) {
        printf("%f WRONG RESULT %i", C[i], i);
        break;
        }
    }

    free(A);
    free(B);
    free(C);

    printf("\n***********\n");
    printf("* SUCCESS *\n");
    printf("***********\n\n");

    Py_INCREF(Py_None);
    return Py_None;
}