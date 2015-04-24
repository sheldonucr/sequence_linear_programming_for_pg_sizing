#include "Dcompact.h"

void mps_gen(EQU_LIST **equa_b,RHS_LIST *rhs_values)

{
 int coeff, ligne, i, j, new_X, new_X1, new_X2;
 RHS_ITEM *rhs_i;
 EQU_ITEM *equa_iter;
 FILE *mps_file, *f_results;
 extern int num_of_var;

 mps_file = fopen("mps.file", "w");

 ligne = coeff = 1;
 fprintf(mps_file,"NAME LINDO GENERATED MPS FILE (MIN)\n");

 /* Generation of the ROWS section*/
 fprintf(mps_file,"ROWS\n");
 fprintf(mps_file,"  N %-5d\n", ligne);

 rhs_i = rhs_values->head;

 while ( rhs_i )
   {
    if (rhs_i->value == 0) 
       fprintf(mps_file,"%-4s%-8d\n","  E",rhs_i->column);
       else
         fprintf(mps_file,"%-4s%-8d\n","  G",rhs_i->column);
    rhs_i = rhs_i->next;
    }
 /* Generation of the columns section*/
 fprintf(mps_file,"COLUMNS\n");
 for (i = 0; i < num_of_var; i++)
     {
      equa_iter = (*equa_b+i)->head;
      fprintf(mps_file,"    X%-7d  %-8d  %d\n",i+1,ligne,coeff);

      while (equa_iter)
         {
          fprintf(mps_file,"    X%-7d  %-8d  %-d\n",i+1,equa_iter->rank,equa_iter->coeff);
          equa_iter = equa_iter->next;
          }
      }
      
 /* Generation of the RHS section*/
 fprintf(mps_file,"RHS\n");
 rhs_i = rhs_values->head;
  
   while ( rhs_i )
      {
        fprintf(mps_file,"    %-8s  %-8d  %d\n","RHS",rhs_i->column,rhs_i->value);
        rhs_i = rhs_i->next;
        }

 fprintf(mps_file,"ENDATA\n");
 fclose(mps_file);
 system("opt -f mps.file");
 system("rpt");
 
 f_results = fopen("mps.file.rpt", "r");


   /*update the edges*/ 
 while (!feof(f_results))
   {
    fscanf(f_results,"%d%d",&i, &new_X);
    (*equa_b+i-1)->the_edge->p1->x = (*equa_b+i-1)->the_edge->p2->x = new_X;
   /*     
    fscanf(f_results,"%d%d",&i, &new_X1);
    fscanf(f_results,"%d%d",&j, &new_X2);

    if (((*equa_b+i-1)->head != NULL) && ((*equa_b+j-1)->head != NULL))
       {
        
        (*equa_b+i-1)->the_edge->p1->x = (*equa_b+i-1)->the_edge->p2->x = new_X1;
        (*equa_b+j-1)->the_edge->p1->x = (*equa_b+j-1)->the_edge->p2->x = new_X2;
        }
        else
         
          if (((*equa_b+i-1)->head == NULL) && ((*equa_b+j-1)->head != NULL))
            {
            (*equa_b+j-1)->the_edge->p1->x = (*equa_b+j-1)->the_edge->p2->x = new_X2;               (*equa_b+i-1)->the_edge->p1->x = (*equa_b+i-1)->the_edge->p2->x =                      new_X2 - theGlobalScale*WIDTH[(*equa_b+i-1)->the_edge->layer_num];
             }
            else
             if (((*equa_b+i-1)->head != NULL) && ((*equa_b+j-1)->head == NULL))
               {
               (*equa_b+i-1)->the_edge->p1->x = (*equa_b+i-1)->the_edge->p2->x =                       new_X1;
               (*equa_b+j-1)->the_edge->p1->x = (*equa_b+j-1)->the_edge->p2->x =                      new_X1 + theGlobalScale*WIDTH[(*equa_b+i-1)->the_edge->layer_num];
                }
                else
                  (*equa_b+j-1)->the_edge->p1->x = (*equa_b+j-1)->the_edge->p2->x =                      (*equa_b+i-1)->the_edge->p1->x+theGlobalScale*WIDTH[(*equa_b+i-1)->the_edge->layer_num];*/
    }
 fclose(f_results);
 }



