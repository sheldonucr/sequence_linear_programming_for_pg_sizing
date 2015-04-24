*constraint file for g3x3_1.sp circuit
*Layer INDEX UNIT_RES MIN_WIDTH MAX_CURRENT_DEN
LAYER 1 1 1 0.001

*voltage constraints
CONST v(1) <= 0.09
CONST v(2) <= 0.09
CONST v(4) <= 0.07
CONST v(6) <= 0.08
CONST v(10) <= 0.06
CONST v(12) <= 0.05
CONST v(14) <= 0.06
CONST v(16) <= 0.011

*width constraints

CONST w(rr11) == w(rr12)
CONST w(rr11) ==  w(rr13)

CONST w(rr41) == w(rr42)
CONST w(rr41) == w(rr43)

CONST w(rc11) == w(rc21)
CONST w(rc11) == w(rc31)

CONST w(rc12) == w(rc22)
CONST w(rc12) == w(rc32)
