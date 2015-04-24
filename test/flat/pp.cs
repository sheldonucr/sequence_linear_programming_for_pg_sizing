*Layer INDEX UNIT_RES MIN_WIDTH MAX_CURRENT_DEN
LAYER 1 1 0.5 0.001

*voltage constraints

CONST v(1) > 4.99 
CONST v(2) > 4.99
CONST v(6) > 4.99
CONST v(10) > 4.98
CONST v(12) > 4.99
CONST v(14) > 4.98
CONST v(7) > 4.98

*width constraints

CONST w(rr11) == w(rr12)
CONST w(rr11) ==  w(rr13)

CONST w(rr41) == w(rr42)
CONST w(rr41) == w(rr43)

CONST w(rc11) == w(rc21)
CONST w(rc11) == w(rc31)

CONST w(rc12) == w(rc22)
CONST w(rc12) == w(rc32)
