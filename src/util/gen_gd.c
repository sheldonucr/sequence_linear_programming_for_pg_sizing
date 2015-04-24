/*
 * $RCSfile: gen_gd.c,v $
 * $Revision: 1.3 $
 * $Date: 2002/05/05 23:31:58 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

#include <stdio.h>
#include <stdlib.h>

#define NC(S) (S + j*sec)

main(int argc, char **argv)
{
    int sec, i, j, seg, strip;
    int rcout = 1;
    int icout = 1;
    char    name[256];
    char    spname[256], csname[256];
    double    offset;
    double  cur_base = 1.23684e-05, cur_var;
    double    res_base = 1.6, res_var;
    double  length,width = 0.8;
    FILE    *fp;

    setbuf(stdout,0);
    printf("#segment(#(row):");
    scanf("%d",&seg);
    printf("# R-I section in each segment:");
    scanf("%d",&sec);
	printf("# strip in each segment:");
    scanf("%d",&strip);
    printf("fileroot:");
    scanf("%s", name);
    printf("voltage offset:");
    scanf("%lf",&offset);

	printf("#seg(#row): %d\n",seg);
    printf("#sec(R-I section): %d\n",sec);
	printf("#vertical strip: %d\n",strip);

    printf("filename: %s\n",name);
    printf("voltage offset: %g\n",offset);

	if(strip >= sec)
	{
		printf("Invalid p/g strip number %d", strip);
		exit(-1);
	}

    sprintf(spname,"%s.sp",name);
    sprintf(csname,"%s.cs",name);

    /* first, the netlist file */
    fp = fopen(spname,"w");
    if(!fp)
	{
        perror(spname);
        exit(-1);
    }

    fprintf(fp,"num_node %d\n",sec*seg);
    fprintf(fp,"matrix_size %d\n\n",sec*seg);

    fprintf(fp,"* %d x %d ground network\n",seg,sec);
	
	for(j = 0; j <  seg; j++)
	{

		fprintf(fp,"*seg: %d\n",j+1); 
		 /* inter-segment (strip) connection */
        if((seg - j) > 1)
		{
			int interval = sec/strip;
			if(interval < 3)
			{
				printf("Serise chain circuit is too small: %d", interval);
				exit(-1);
			}

			fprintf(fp,"* inter-segment connection\n");
			for( i = 0; i < strip; i++)
			{			
				fprintf(fp,
						"R%d %d %d 6.16 0.8 49.28 37\n",rcout++, 
						j*sec+interval*(i+1),
						(j+1)*sec + interval*(i+1));
			}
		}
        
        fprintf(fp,
            "R%d %d %d 2.5 0.8 20 37\n",rcout++, 0, j*sec+1 );
       
        for( i = 1; i < sec; i++)
		{
			cur_var = cur_base*((rand()%10)/100.0);
			fprintf(fp,"I%d %d %d %g\n",icout++, 0, NC(i),
					cur_base + cur_var);
			res_var = res_base*((rand()%10)/100.0);
			length = width*(res_var + res_base)*10.0;
			fprintf(fp,"R%d %d %d %g %g %g 37\n",
					rcout++, NC(i), NC(i+1),res_base+res_var, width, length);
        }
        fprintf(fp,"I%d %d %d 1.23684e-05\n",icout++, 0, NC(i));
        fprintf(fp,"R%d %d %d 2.5 0.8 20 37\n",rcout++, NC(i), 0);
    }

    fprintf(fp,"\n.op");
    fprintf(fp,"\n.end");
    fclose(fp);

    /* the constraint file */
    fp = fopen(csname,"w");
    if(!fp)
	{
        perror(csname);
        exit(-1);
    }
    
	fprintf(fp,"*layer information\n");
    fprintf(fp,"*Layer INDEX UNIT_RES MIN_WIDTH MAX_CURRENT_DEN\n");
    fprintf(fp,"LAYER 37 0.1 0.4 1.7\n");

    fprintf(fp,"\n*voltage constraint\n");
    for( i = 1; i <= seg*sec; i++)
	{
        fprintf(fp,"CONST v(%d) <= %g\n",i,offset);
	}
    
    fclose(fp);
}
