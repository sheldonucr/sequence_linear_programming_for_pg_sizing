/*
 * $RCSfile: pgWriteSpice.c,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:07:28 $, (c) Copyright 1999 by X.-D. Sheldon, Tan
 */

/*
**     output the SPICE netlist from the physical
**    netlist.
*/

#include "pgCommon.h"
#include "pgDoor.h"
#include <math.h>

void 
pgDumpSpiceFile(FILE *spfile)
{
    PhyResistor *raux;
    IndpSource *caux;
    double    val;
    Layer *lay;

    assert(thePhyResList);

    /* the size the matrix and #node */
    fprintf(spfile,"num_node %d\n",theIntNodeCount-1);
    fprintf(spfile,"matrix_size %d\n",theIntNodeCount-1);

    /* first dump the resistor netlist */
    fprintf(spfile,"\n* resistor network\n");
    fprintf(spfile,"\n* Rxxx n1 n2 value width length layer\n");
    for(raux = thePhyResList; raux; raux = raux->next){
        lay = pgFindLayerByIndex(raux->layer);
        if(!lay){
            sprintf(buf,"Cannot find definition for layer %d.",
            raux->layer);
            error_mesg(FAT_ERROR,buf);
            exit(-1);
        }
        if(raux->dy > raux->dx){
            if(raux->dx/lengthPrecision <= lay->minWidth){
                raux->dx = 2*lay->minWidth*lengthPrecision;
                raux->dy = 2*raux->dy;
            }
            val = raux->dy/raux->dx;
            /* resistor unit: ohm */
            val = val*lay->unitRes*unitResistance;
            fprintf(spfile,"R%ld %d %d %g %g %g %d \n",
                raux->index,
                raux->n1, raux->n2, val,
                raux->dx/lengthPrecision,
                raux->dy/lengthPrecision,
                raux->layer);
        }
        else {
            if(raux->dy/lengthPrecision <= lay->minWidth){
                raux->dy = 2*lay->minWidth*lengthPrecision;
                raux->dx = 2*raux->dx;
            }
            val = raux->dx/raux->dy;
            val = val*lay->unitRes*unitResistance;
            fprintf(spfile,"R%ld %d %d %g %g %g %d \n",
                raux->index,
                raux->n1, raux->n2, val,
                raux->dy/lengthPrecision, 
                raux->dx/lengthPrecision,
                raux->layer);
        }
    }

    /* the the current source */
    fprintf(spfile,"\n* indenpendent current source \n");
    /* current unit: mA */
    for(caux = theCurSource; caux; caux = caux->next){
        val = caux->value;
        fprintf(spfile,"I%ld %d %d %g\n",caux->index,
            caux->nn, caux->pn, val);
    }

    /* the the voltage source for power newtork*/
    /* voltage unit: mV */
    /*
    if(pgType == cPOWER)
        fprintf(spfile,"\nVDD %d %d %g\n", 1, 0, theVoltage*1000);
    */

    /* the end statement */
    fprintf(spfile,"\n.op\n"); 
    fprintf(spfile,".end\n"); 

}
