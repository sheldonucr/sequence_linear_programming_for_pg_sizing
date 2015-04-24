*Layer INDEX UNIT_RES MIN_WIDTH MAX_CURRENT_DEN
LAYER 1 1 0.001 0.001

*voltage constraints

CONST v(1) < 0.0039
CONST v(2) < 0.0079
CONST v(6) < 0.01
CONST v(10) < 0.015
CONST v(12) < 0.005 
CONST v(14) < 0.012
CONST v(7) < 0.011
CONST v(5) < 0.0001

*width constraints

#CONST w(rr11) == w(rr12)
#CONST w(rr11) ==  w(rr13)

#CONST w(rr41) == w(rr42)
#CONST w(rr41) == w(rr43)

#CONST w(rc11) == w(rc21)
#CONST w(rc11) == w(rc31)

#CONST w(rc12) == w(rc22)
#CONST w(rc12) == w(rc32)

