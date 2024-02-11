from ctypes import c_double, c_size_t, CDLL, POINTER, Structure, pointer
import os
import platform

_file_path, _ = os.path.split(__file__)
if platform.system() == "Windows":
    _nsfd_lib_path = os.path.join(_file_path, "nsfd.pymem.dll")
elif platform.system() == "Darwin":
    _nsfd_lib_path = os.path.join(_file_path, "libnsfd.pymem.dylib")
elif platform.system() == "Linux":
    _nsfd_lib_path = os.path.join(_file_path, "libnsfd.pymem.so")

_nsfd_lib = CDLL(_nsfd_lib_path)

Real = c_double
Real_p = POINTER(Real)


class GridShape(Structure):
    _fields_ = [("m_rows", c_size_t), ("n_cols", c_size_t)]


class ScalarField(Structure):
    pass


ScalarFieldPtr = POINTER(ScalarField)

s_field_new = _nsfd_lib.nsfd_s_field_new
s_field_new.restype = ScalarFieldPtr
s_field_new.argtypes = [c_size_t, c_size_t]

s_field_free = _nsfd_lib.nsfd_s_field_free
s_field_free.restype = None
s_field_free.argtypes = [POINTER(ScalarFieldPtr)]

s_field_shape = _nsfd_lib.nsfd_s_field_shape
s_field_shape.restype = GridShape
s_field_shape.argtypes = [ScalarFieldPtr]

s_field_init_const = _nsfd_lib.nsfd_s_field_init_const
s_field_init_const.restype = None
s_field_init_const.argtypes = [ScalarFieldPtr, Real]

s_field_init_grid = _nsfd_lib.nsfd_s_field_init_grid
s_field_init_grid.restype = None
s_field_init_grid.argtypes = [ScalarFieldPtr, Real, Real, Real, Real]

s_field_get_values = _nsfd_lib.nsfd_s_field_get_values
s_field_get_values.restype = None
s_field_get_values.argtypes = [ScalarFieldPtr, Real_p]

s_field_get_grid = _nsfd_lib.nsfd_s_field_get_grid
s_field_get_grid.restype = None
s_field_get_grid.argtypes = [ScalarFieldPtr, Real_p, Real_p]

if __name__ == "__main__":
    import numpy as np

    m_rows = 11
    n_cols = 11

    x_min = -5
    x_max = 5
    y_min = 0
    y_max = 10

    dx = (x_max - x_min) / (n_cols - 1)
    dy = (y_max - y_min) / (m_rows - 1)

    field_x_min = x_min + dx / 2
    field_x_max = x_max - dx / 2
    field_y_min = y_min + dy / 2
    field_y_max = y_max - dy / 2

    p_ptr = s_field_new(m_rows, n_cols)

    s_field_init_const(p_ptr, Real(1))
    s_field_init_grid(p_ptr, x_min, x_max, y_min, y_max)

    field_shape = s_field_shape(p_ptr)
    p_values = np.empty((field_shape.m_rows, field_shape.n_cols))
    x_grid = np.empty_like(p_values)
    y_grid = np.empty_like(p_values)

    s_field_get_values(p_ptr, p_values.ctypes.data_as(Real_p))
    s_field_get_grid(
        p_ptr, x_grid.ctypes.data_as(Real_p), y_grid.ctypes.data_as(Real_p)
    )

    s_field_free(pointer(p_ptr))
