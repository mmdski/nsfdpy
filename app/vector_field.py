from nsfdpy.field import VectorField


vf = VectorField(10, 15)
vf[5, 7] = (1, 2)
values = vf.values()
