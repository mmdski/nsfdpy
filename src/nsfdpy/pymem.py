from ctypes import c_size_t, c_void_p, pythonapi

malloc = pythonapi.PyMem_RawMalloc
malloc.argtypes = [c_size_t]
malloc.restype = c_void_p

free = pythonapi.PyMem_RawFree
free.argtypes = [c_void_p]
free.restype = None
