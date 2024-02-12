from ctypes import c_double, c_float, c_size_t, CDLL, POINTER, Structure, pointer
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

nsfd_sizeof_real = _nsfd_lib.nsfd_sizeof_real
nsfd_sizeof_real.restype = c_size_t
nsfd_sizeof_real.argtypes = None

if nsfd_sizeof_real() == 8:
    Real = c_double
elif nsfd_sizeof_real() == 4:
    Real = c_float

Real_p = POINTER(Real)


class NSFDGridShape(Structure):
    _fields_ = [("m_rows", c_size_t), ("n_cols", c_size_t)]


class NSFDGrid(Structure):
    pass


NSFDGridPtr = POINTER(NSFDGrid)

grid_new = _nsfd_lib.nsfd_grid_new
grid_new.restype = NSFDGridPtr
grid_new.argtypes = [c_size_t, c_size_t]

grid_free = _nsfd_lib.nsfd_grid_free
grid_free.restype = None
grid_free.argtypes = [POINTER(NSFDGridPtr)]

grid_shape = _nsfd_lib.nsfd_grid_shape
grid_shape.restype = NSFDGridShape
grid_shape.argtypes = [NSFDGridPtr]

grid_init_const = _nsfd_lib.nsfd_grid_init_const
grid_init_const.restype = None
grid_init_const.argtypes = [NSFDGridPtr, Real]

grid_init_grid = _nsfd_lib.nsfd_grid_init_grid
grid_init_grid.restype = None
grid_init_grid.argtypes = [NSFDGridPtr, Real, Real, Real, Real]

grid_get_values = _nsfd_lib.nsfd_grid_get_values
grid_get_values.restype = None
grid_get_values.argtypes = [NSFDGridPtr, Real_p]

grid_get_coords = _nsfd_lib.nsfd_grid_get_coords
grid_get_coords.restype = None
grid_get_coords.argtypes = [NSFDGridPtr, Real_p, Real_p]

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

    p_ptr = grid_new(m_rows, n_cols)

    grid_init_const(p_ptr, Real(1))
    grid_init_grid(p_ptr, x_min, x_max, y_min, y_max)

    field_shape = grid_shape(p_ptr)
    p_values = np.empty((field_shape.m_rows, field_shape.n_cols))
    x_grid = np.empty_like(p_values)
    y_grid = np.empty_like(p_values)

    grid_get_values(p_ptr, p_values.ctypes.data_as(Real_p))
    grid_get_coords(p_ptr, x_grid.ctypes.data_as(Real_p), y_grid.ctypes.data_as(Real_p))

    grid_free(pointer(p_ptr))
