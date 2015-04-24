*num_node 16
*matrix_size 19

* name, n1, n2, value, width, length, lay_index
rr11	5 2 10 1 10 1
rr12 	2 4 10 1 10 1
rc12 	4 8 5 1 5 1
rr23	5 8 24 1 24 1
rc21	5 9 5 1 5 1
rc22	8 12 5 1 5 1
rr31	9 12 24 1 24 1
rc31	9 13 5 1 5 1
rc32 	12 16 5 1 5 1
rr41	13 16 15 1 15 1

* current sources 
I1_1 5 0 1.5mA
I2   2 0  2mA 
I3_1 4 0 0.5mA
I6 5 0  1mA
I7 8 0  1mA
I10 9 0  1mA
I11 12 0  1mA
I13 13 0 2mA
I15  16 0  1mA

* voltage sources
V4 4 0 5
V5 5 0 5
V16 16 0 5

.op
.end
