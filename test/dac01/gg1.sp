*num_node 3
*matrix_size 3

* name, n1, n2, value, width, length, lay_index
rr11	1 2 1 1 10 1
rr12 	2 3 5 1 10 1
rr13	3 0 5 1 10 1
rc11	1 0 5 1 10 1

* current sources 
I1 0 1  2
I2 0 2  2 
I3 0 3  3

* voltage sources
*V4 4 0 5
*V5 5 0 5
*V16 16 0 5

.op
.sens v(1) v(2)
.end
