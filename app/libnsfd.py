from ctypes import cast, pythonapi

from nsfdpy import nsfd


grid_shape = nsfd.GridShape(10, 15)
s_field_size = nsfd.s_field_size(grid_shape)
s_field_ptr = cast(pythonapi.PyMem_Malloc(s_field_size), nsfd.ScalarFieldPtr)
if not s_field_ptr:
    raise MemoryError
s_field_ptr = nsfd.s_field_init(grid_shape, s_field_ptr, nsfd.Real(0))

# pythonapi.PyMem_Free(s_field_ptr)
# s_field_ptr = None
