from ctypes import cast

from nsfdpy import nsfd, pymem


shape = nsfd.DomainShape(10, 15)
s_field_size = nsfd.s_field_size(shape)
s_field_ptr = cast(pymem.malloc(s_field_size), nsfd.ScalarFieldPtr)
if s_field_ptr is None:
    raise MemoryError
s_field_ptr = nsfd.s_field_init(shape, s_field_ptr, nsfd.Real(0))

pymem.free(s_field_ptr)
s_field_ptr = None
