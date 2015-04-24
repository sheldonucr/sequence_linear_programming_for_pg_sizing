*num_node 16
*matrix_size 16

rr11	1 2 5 1 10 1
rr12 	2 3 5 1 10 1
rr13	3 0 5 1 10 1
rc11	1 0 5 1 10 1
rc12 	0 8 5 1 10 1
rr21	0 6 8 1 10 1
rr22	6 7 8 1 10 1
rr23	7 8 8 1 10 1
rc21	0 9 5 1 10 1
rc22	8 12 5 1 10 1
rr31	9 10 8 1 10 1
rr32	10 11 8 1 10 1
rr33	11 12 8 1 10 1
rc31	9 13 5 1 10 1
rc32 	12 0 5 1 10 1
rr41	13 14 5 1 10 1
rr42	14 15 5 1 10 1
rr43	15 0 5 1 10 1

* current sources 
I2 0 2  1mA 
I3 0 3  1mA
I6 0 6  1mA
I7 0 7  1mA
I10 0 10  1mA
I11 0 11  1mA
I14  0 14  1mA
I15  0 15   1mA

* voltage sources

.op
.sens v(1) v(2)
.end
