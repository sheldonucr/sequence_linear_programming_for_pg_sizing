*num_node 16
*matrix_size 19

* name, n1, n2, value, width, length, lay_index
rr11	1 2 5 1 5 1
rr12 	2 3 5 1 5 1
rr13	3 4 5 1 5 1
rc11	1 5 5 1 5 1
rc12 	4 8 5 1 5 1
rr21	5 6 8 1 8 1
rr22	6 7 8 1 8 1
rr23	7 8 8 1 8 1
rc21	5 9 5 1 5 1
rc22	8 12 5 1 5 1
rr31	9 10 8 1 8 1
rr32	10 11 8 1 8 1
rr33	11 12 8 1 8 1
rc31	9 13 5 1 5 1
rc32 	12 16 5 1 5 1
rr41	13 14 5 1 5 1
rr42	14 15 5 1 5 1
rr43	15 16 5 1 5 1

* current sources 
I2 2 0  1mA 
I3 3 0  1mA
I6 6 0  1mA
I7 7 0  1mA
I10 10 0  1mA
I11 11 0  1mA
I14 14 0  1mA
I15  15 0  1mA

* voltage sources
V4 4 0 5
V5 5 0 5
V16 16 0 5

.op
.sens v(1) v(2)
.end
