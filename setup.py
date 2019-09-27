import os
import sys
from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension

import os
import sys


def set_cc_variables(var_name="CC"):
    cc_var = os.environ.get(var_name)
    if cc_var is not None:
        os.environ["CC"] = cc_var
        if sys.platform == "darwin":
            os.environ["LDSHARED"] = (
                cc_var + " -bundle -undefined dynamic_lookup")
        else:
            os.environ["LDSHARED"] = cc_var + " -shared"
    return cc_var


def get_openmp_flag():
    if sys.platform == "win32":
        return ["/openmp"]
    elif sys.platform == "darwin" and 'openmp' in os.getenv('CPPFLAGS', ''):
        return []
    return ["-fopenmp"]


original_environ = os.environ.copy()
try:
    set_cc_variables("CC")
    openmp_flag = get_openmp_flag()

    ext_modules = [
        Extension(
            "prange_blas",
            ["prange_blas.pyx"],
            extra_compile_args=openmp_flag,
            extra_link_args=openmp_flag,
            libraries=['mkl_rt']
            )
    ]

    setup(
        ext_modules=cythonize(
            ext_modules,
            compiler_directives={'language_level': 3,
                                 'boundscheck': False,
                                 'wraparound': False})
    )

finally:
    os.environ.update(original_environ)
