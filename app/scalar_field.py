from nsfdpy.field import ScalarField


sf = ScalarField(10, 15)
sf[5, 7] = 1
values = sf.values()
