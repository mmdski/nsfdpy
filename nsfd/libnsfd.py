from ctypes import (
    c_double,
    c_float,
    c_size_t,
    CDLL,
    POINTER,
    Structure,
    sizeof,
)
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

if nsfd_sizeof_real() == sizeof(c_double):
    Real = c_double
elif nsfd_sizeof_real() == sizeof(c_float):
    Real = c_float
else:
    raise RuntimeError(f"Unknown size of NSFDReal: {nsfd_sizeof_real()}")

RealPtr = POINTER(Real)


class GridGeomData(Structure):
    _fields_ = [
        ("imax", c_size_t),
        ("jmax", c_size_t),
        ("xlength", Real),
        ("ylength", Real),
    ]


class GridGeom(Structure):
    pass


GridGeomDataPtr = POINTER(GridGeomData)
GridGeomPtr = POINTER(GridGeom)

grid_geom_new = _nsfd_lib.nsfd_grid_geom_new
grid_geom_new.restype = GridGeomPtr
grid_geom_new.argtypes = [GridGeomDataPtr]

grid_geom_free = _nsfd_lib.nsfd_grid_geom_free
grid_geom_free.restype = None
grid_geom_free.argtypes = [POINTER(GridGeomPtr)]

grid_geom_n_cells = _nsfd_lib.nsfd_grid_geom_n_cells
grid_geom_n_cells.restype = c_size_t
grid_geom_n_cells.argtypes = [GridGeomPtr]

grid_geom_init_p = _nsfd_lib.nsfd_grid_geom_init_p
grid_geom_init_p.restype = None
grid_geom_init_p.argtypes = [GridGeomPtr]

grid_geom_init_u = _nsfd_lib.nsfd_grid_geom_init_u
grid_geom_init_u.restype = None
grid_geom_init_u.argtypes = [GridGeomPtr]

grid_geom_init_v = _nsfd_lib.nsfd_grid_geom_init_v
grid_geom_init_v.restype = None
grid_geom_init_v.argtypes = [GridGeomPtr]

grid_geom_coords = _nsfd_lib.nsfd_grid_geom_coords
grid_geom_coords.restype = None
grid_geom_coords.argtypes = [GridGeomPtr, RealPtr, RealPtr]


if __name__ == "__main__":
    from ctypes import pointer

    import numpy as np

    imax = 10
    jmax = 10
    xlength = 10
    ylength = 10

    geom_data = GridGeomData(imax, jmax, xlength, ylength)
    grid_geom_p = grid_geom_new(pointer(geom_data))

    grid_geom_free(grid_geom_p)
