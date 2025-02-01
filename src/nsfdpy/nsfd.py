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
    _nsfd_lib_path = os.path.join(_file_path, "nsfd.dll")
elif platform.system() == "Darwin":
    _nsfd_lib_path = os.path.join(_file_path, "libnsfd.dylib")
elif platform.system() == "Linux":
    _nsfd_lib_path = os.path.join(_file_path, "libnsfd.so")

_nsfd_lib = CDLL(_nsfd_lib_path)

ns_real_size = _nsfd_lib.ns_real_size
ns_real_size.restype = c_size_t

Real: type[c_double] | type[c_float]

if ns_real_size() == sizeof(c_double):
    Real = c_double
elif ns_real_size() == sizeof(c_float):
    Real = c_float
else:
    raise RuntimeError(f"Unknown size of real: {ns_real_size()}")

RealPtr = POINTER(Real)


class Vector(Structure):
    _fields_ = [("x", Real), ("y", Real)]


VectorPtr = POINTER(Vector)


class DomainShape(Structure):
    _fields_ = [("imax", c_size_t), ("jmax", c_size_t)]


class GridShape(Structure):
    _fields_ = [("nx", c_size_t), ("ny", c_size_t)]


# scalar field
class ScalarField(Structure):
    pass


ScalarFieldPtr = POINTER(ScalarField)

s_field_size = _nsfd_lib.ns_s_field_mem_size
s_field_size.argtypes = [DomainShape]
s_field_size.restype = c_size_t

s_field_init = _nsfd_lib.ns_s_field_init
s_field_init.argtypes = [DomainShape, ScalarFieldPtr, Real]
s_field_init.restype = ScalarFieldPtr

s_field_shape = _nsfd_lib.ns_s_field_shape
s_field_shape.argtypes = [ScalarFieldPtr]
s_field_shape.restype = DomainShape

s_field_grid_shape = _nsfd_lib.ns_s_field_grid_shape
s_field_grid_shape.argtypes = [ScalarFieldPtr]
s_field_grid_shape.restype = GridShape

s_field_get = _nsfd_lib.ns_s_field_get
s_field_get.argtypes = [ScalarFieldPtr, c_size_t, c_size_t]
s_field_get.restype = RealPtr

s_field_set = _nsfd_lib.ns_s_field_set
s_field_set.argtypes = [ScalarFieldPtr, c_size_t, c_size_t, Real]
s_field_set.restype = None

s_field_values = _nsfd_lib.ns_s_field_values
s_field_values.argtypes = [ScalarFieldPtr, RealPtr]
s_field_values.restype = None

# vector field


class VectorField(Structure):
    pass


VectorFieldPtr = POINTER(VectorField)


v_field_size = _nsfd_lib.ns_v_field_mem_size
v_field_size.argtypes = [DomainShape]
v_field_size.restype = c_size_t

v_field_init = _nsfd_lib.ns_v_field_init
v_field_init.argtypes = [DomainShape, VectorFieldPtr, Vector]
v_field_init.restype = VectorFieldPtr

v_field_shape = _nsfd_lib.ns_v_field_shape
v_field_shape.argtypes = [VectorFieldPtr]
v_field_shape.restype = DomainShape

v_field_grid_shape = _nsfd_lib.ns_v_field_grid_shape
v_field_grid_shape.argtypes = [VectorFieldPtr]
v_field_grid_shape.restype = GridShape

v_field_get = _nsfd_lib.ns_v_field_get
v_field_get.argtypes = [VectorFieldPtr, c_size_t, c_size_t]
v_field_get.restype = VectorPtr

v_field_set = _nsfd_lib.ns_v_field_set
v_field_set.argtypes = [VectorFieldPtr, c_size_t, c_size_t, Real, Real]
v_field_set.restype = None

v_field_values = _nsfd_lib.ns_v_field_values
v_field_values.argtypes = [VectorFieldPtr, RealPtr, RealPtr]
v_field_values.restype = None
