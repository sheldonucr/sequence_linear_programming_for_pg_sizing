/*
 * $RCSfile: gen_pr.c,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/05 23:31:00 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

#include <stdio.h>
#include <stdlib.h>

#define I_UNIT (0.35) // current unit in mA
#define C_UNIT (100e-12) // capacitance per um in F
#define L_UNIT (1.343e-12) // inductance of 1x1x1 um^3 volumn in henry at 1Ghz
#define CONDUCTIVITY (1.76)  // conductivity of AL in (micron-ohm-cm)^-1
//#define R_UNIT  4*(CONDUCTIVITY*1e-2) // resistance of 1x1x1 um^3 volumn
#define R_UNIT  (CONDUCTIVITY*0.1)
#define NC(S) (S + j*2*(sec+1))

main(int argc, char **argv)
{
	int sec, i, j, row,strip;
	int rcout = 1;
	int ccout = 1;
	int icout = 1;
	int lcout = 1;
	int n1, n2;
	char    name[256];
	double  res_val, cap_val, ind_val;
	double  I1, I2, I3, I4, I5;
	double  voltage;
	double  cur_var;
	FILE    *fp;
  
	setbuf(stdout,0);
	printf("# row:");
	scanf("%d",&row);
	printf("# RC/RLC sections in each row:");
	scanf("%d",&sec);
	printf("# strips in each row:");
	scanf("%d",&strip);
  
	printf("filename:");
	scanf("%s", name);
	printf("voltage source:");
	scanf("%lf",&voltage);
 
	printf("#sec: %d\n",sec);
	printf("#row: %d\n",row);
	printf("#vertical strip: %d\n",strip);
  
	printf("filename: %s\n",name);
	printf("voltage: %g\n",voltage);
 
	if(strip >= 2*sec)
    {
		printf("Invalid p/g strip number %d", strip);
		exit(-1);
    }
   
	/* first, the netlist file */
	fp = fopen(name,"w");
	if(!fp)
    {
		perror(name);
		exit(-1);
    }

	fprintf(fp,"\n** supply voltage: %g\n", voltage);
	fprintf(fp,"** #row: %d\n", row);
	fprintf(fp,"** #RC/RLC section per row: %d\n", sec);
	fprintf(fp,"** # of p/g strips per row: %d\n", strip);
	fprintf(fp,"** num_node %d\n",2*sec*row+1);
  
	fprintf(fp,"** This is a %d x %d power network\n",row,sec);
	for(j = 0; j <  row; j++)
    {
		fprintf(fp,"** row: %d\n",j+1); 
		/* inter-row (strip) connection */
		if((row - j) > 1)
		{
			int interval = 2*sec/strip;
			if(interval >= 3)
			{
	      
				fprintf(fp,"** p/g strip connection\n");
				for( i = 0; i < strip; i++)
				{			
					//p/g strip has smaller resistance as they are wider.
					res_val = 4*R_UNIT*(1 + ((rand()%10)/100.0));		  
					// n1 and n2 must be odd number
					n1 = (j*2*sec + interval*(i+1));
					n1 = n1%2?n1:n1-1;
					n2 = (j+1)*2*sec + interval*(i+1);
					n2 = n2%2?n2:n2-1;
					fprintf(fp,"R%d %d %d %g\n",
							rcout++, 
							n1,
							n2,
							res_val);
				}
			}	
			else
			{
				printf("P/G inseration is ignored as sec/strip is too small: %d\n", interval);	      
			}
			fprintf(fp,"** end of p/g strip connection\n");
		}
	        
		// connect row to the power supply
		res_val = 0.01*R_UNIT*(1 + ((rand()%10)/100.0));
		fprintf(fp,"R%d %d %d %g\n",
				rcout++, 	     
				j*2*(sec+1)+1,
				2*(sec+1)*row+1, 
				res_val );
            
		for( i = 1; i < 2*sec; i = i + 2)
		{
	  
			// time varying current sources
			I1 = I_UNIT*(((rand()%100)/100.0));
			I2 = I_UNIT*(1.3 + ((rand()%100)/100.0));
			I3 = I_UNIT*(1 + ((rand()%90)/100.0));
			I4 = I_UNIT*(0.5 + ((rand()%80)/100.0));
			I5 = I_UNIT*(((rand()%10)/100.0));
			fprintf(fp,"I%d %d %d DC 0 PWL(0 0.0 1ns %gmA 2ns %gmA 3ns %gmA 4ns %gmA 5ns %gmA)\n",
					icout++, NC(i), 0, 
					I1, I2, I3, I4, I5);

			// capacitance
			cap_val = C_UNIT*(1 + ((rand()%100)/100.0));
			fprintf(fp,"C%d %d %d %g\n",
					ccout++, NC(i), 0, cap_val); 
		  
			// resistance
			res_val = R_UNIT*(1 + ((rand()%10)/100.0));
			fprintf(fp,"R%d %d %d %g\n",
					rcout++, NC(i), NC(i+1),res_val);

			// inductance
			ind_val = L_UNIT*(1 + ((rand()%10)/100.0));
			fprintf(fp,"L%d %d %d %g\n",
					lcout++, NC(i+1), NC(i+2), ind_val);
		}
      
		I1 = I_UNIT*(((rand()%100)/100.0));
		I2 = I_UNIT*(1 + ((rand()%100)/100.0));
		I3 = I_UNIT*(1 + ((rand()%90)/100.0));
		I4 = I_UNIT*(0.5 + ((rand()%80)/100.0));
		I5 = I_UNIT*(((rand()%10)/100.0));
		fprintf(fp,"I%d %d %d DC 0 PWL(0 0.0 1ns %gmA 2ns %gmA 3ns %gmA 4ns %gmA 5ns %gmA)\n",
				icout++, NC(i), 0,
				I1, I2, I3, I4, I5);
      
		// capacitance at one end.
		cap_val = C_UNIT*(1 + ((rand()%100)/100.0));
		fprintf(fp,"C%d %d %d %g\n",
				ccout++, NC(i), 0, cap_val); 
		  
      
		// connect to the power supply
		res_val = 0.01*R_UNIT*(1 + ((rand()%10)/100.0));
		fprintf(fp,"R%d %d %d %g\n",
				rcout++, NC(i), 
				2*(sec+1)*row+1,
				res_val);      
    }
  
	/* we put a voltage supply at each corner of the chip */
	/*
	  fprintf(fp,"V1 %d %d %g \n", 1, 0, voltage); // r = 0, c = 1
	  fprintf(fp,"V2 %d %d %g \n", 2*sec, 0, voltage); // r = 0 , c = sec
	  fprintf(fp,"V3 %d %d %g \n", (row-2)*2*sec+1, 0, voltage); // r = row-2, c = 1
	  fprintf(fp,"V4 %d %d %g \n", (row-1)*2*sec, 0, voltage); // r = row-1, c = sec
	*/
	fprintf(fp,"\n** supply voltage \n");
	fprintf(fp,"V1 %d %d %g \n", row*2*(sec+1)+1, 0, voltage);
	fprintf(fp,"\n.tran 0.01ns 6n\n");
	fprintf(fp,"\n.end\n");
	fclose(fp);
   
}
