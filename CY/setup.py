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
        ldflags = os.environ.get("LDFLAGS")
        os.environ["LDSHARED"] = cc_var + " -shared " + ldflags
        
    return cc_var


original_environ = os.environ.copy()
try:
    set_cc_variables("CC")

    ext_modules = [
        Extension(
            "cymodule",
            ["cymodule.pyx"],
            extra_compile_args=["-fopenmp"],
            extra_link_args=["-fopenmp"]
            )
    ]

    setup(
        ext_modules=cythonize(
            ext_modules,
            compiler_directives={'language_level': 3,
                                 'boundscheck': False,
                                 'wraparound': False,
                                 'cdivision': True})
    )

finally:
    os.environ.update(original_environ)
