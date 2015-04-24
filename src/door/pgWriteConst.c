
/*
**    Dump out the constraints extracted from 
**    physical network.
*/

/*
 * $RCSfile: pgWriteConst.c,v $
 * $Revision: 1.1 $
 * XiangDong Tan (c) Copyright $Date: 1999/04/30 17:07:28 $
 */

#include "pgCommon.h"
#include "pgDoor.h"
#include <math.h>

void
pgDumpConstrainFile( FILE *fname)
{

    double  vth;
    Layer *layer;
    PhyResistor *resist;
    long    i;

    fprintf(fname, "\n*layer information\n");
    fprintf(fname, "*Layer INDEX UNIT_RES MIN_WIDTH MAX_CURRENT_DEN\n");
    for(layer = theLayerTable; layer; layer = layer->next){
        fprintf(fname, "LAYER %d %g %g %g\n",
            layer->index,
            layer->unitRes*unitResistance,
            layer->minWidth,
            layer->maxCurDen);
    }

    fprintf(fname, "\n*voltage constraint\n");
    vth = fabs(theVoltage - theVolOffset);
    for( i = 1; i < MaxExtNodeIndex; i++){
        if(theConvTable[i] != -1){
            if(pgType == cPOWER)
                fprintf(fname, "CONST v(%ld) <= %g\n", 
                    theConvTable[i],vth*1000); 
            else if(pgType == cGROUND)
                fprintf(fname, "CONST v(%ld) <= %g\n", 
                    theConvTable[i],theVolOffset*1000); 
        }
    }

}
