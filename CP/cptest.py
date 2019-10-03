import sys


class RTLD_GLOBAL_for_import(): 
    def __init__(self): 
        self.saved_rtld = None 
 
    def __enter__(self): 
        import ctypes 
        try: 
            self.saved_rtld = sys.getdlopenflags() 
            sys.setdlopenflags(self.saved_rtld | ctypes.RTLD_GLOBAL) 
        except AttributeError: 
            pass 
        del ctypes 
 
    def __exit__(self, *args): 
        if self.saved_rtld: 
            sys.setdlopenflags(self.saved_rtld) 
            self.saved_rtld = None


with RTLD_GLOBAL_for_import():
    from cpmodule import prange_blas

prange_blas(4)


# from pprint import pprint
# from threadpoolctl import threadpool_info
# pprint(threadpool_info())