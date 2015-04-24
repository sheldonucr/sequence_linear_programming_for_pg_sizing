/*
 * $RCSfile: pgExtConst.c,v $                                                                   
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*     Parse the external constaint files 
*    and build the data structure.
*******************************************************************************/

#include "pgOptmz.h"

/* parse the constrain file */
/*
    Constraint Format in PGOPT system
     
    We use fixed style. Line begin with '#' are comments.
    The first word of each line are the key word.

    we have following statements:

    LAYER layer_index unit_res min_width max_current_density
    CONST var1 RELATION var2/value 
    VAR name resistor_length min_width layer_index 
     
    For CONST type statement. the the left hand side of the expression 
    must be varibles. The numerical value can only appear on the right 
    hand side.

    variables can consist of
            (1) voltage variable with node in parentheses.
            No spaces allowed in bewteen.
            example: CONST v(1) > 4.99
            (2) branch width variable with branch name in parentheses.
            No spaces allowed in bewteen.
            exmaple: 
            CONST w(r2) == w(r14)
            CONST w(r2) >= 0.9n
        RELATION supported in branch width are:
        "==" -- Equal
        ">" or ">=" -- great or great or equal. There are same
        internally.
*/

void pgReadConstFile(Circuit *ckt, char *file )
{
    FILE    *fp;
    char     sbuf[1024], backbuf[1024], name[128];
    char     *line, *pstr, *pstr1, *pstr2, *pstr3;
    int    node;
    ExtConst  *caux;
    Branches *baux;
    int    count = 0;

    assert(ckt);

    if(!file){
        error_mesg(IO_ERROR,"No constrain file name given.\n");
        return;
    }

       if((fp = fopen(file,"r")) == NULL){
                sprintf(buf, "Can not open constrain file: %s.\n",file);
                error_mesg(IO_ERROR,buf);
                return;
        }

    if(theLayerTable)
        pgFreeLayer(theLayerTable);
    theLayerTable = NULL;
        
    while((pstr = fgets(sbuf,1023,fp)) != NULL){

        /* increase the line count */
        count++;

        /* all the characters are transfered to 
           upper case as compitable with SPICE
           deck convension.
        */

        /* back up for diagnosis purpose */
        strcpy(backbuf,sbuf); 
        line = ToUpper(sbuf);

        pstr = NextField(&line);

        if(!pstr) /* skip the black line */    
            continue;

        /* the resistivity */
        if(!strcmp("LAYER",pstr)){
            pstr = NextField(&line);
            if(!pstr)
                goto syntax_error;
            pstr1 = NextField(&line);
            if(!pstr1)
                goto syntax_error;
            pstr2 = NextField(&line);
            if(!pstr2)
                goto syntax_error;
            pstr3 = NextField(&line);
            if(!pstr3)
                goto syntax_error;
            pgNewLayer("Layer",pstr, pstr1, pstr2, pstr3);
        }

        /* constraint statements */ 
        else if(!strcmp("CONST",pstr)){

            caux = pgNewConst();
            pgAddExtConst(ckt, caux);

            /* first variable, must be string type */
            pstr = NextField(&line);
            switch(pstr[0]){

                case 'V': /* voltage topic */
                    caux->topic = cVOL;
                    pstr1 = strchr(pstr,')');
                    if(!pstr1)
                        goto syntax_error;
                    *pstr1 = ' ';
                    pstr1 = strchr(pstr,'(');
                    if(!pstr1)
                        goto syntax_error;
                    sscanf(++pstr1, "%d",&node);
                    caux->node1 = node;

                    /*printf("node: %d\n",node);*/

                    if(node > ckt->nMatrixSize){
                        sprintf(buf,
                        "Line: %d, node: %d is out of range.\n", count,node);
                        error_mesg(PARSE_ERROR,buf);
                        continue;
                    }

                    pstr1 = NextField(&line);
                    if(!pstr1)
                        goto syntax_error;
                    if(!strcmp(pstr1,"=="))
                        caux->type = EQ;
                    else if(!strcmp(pstr1,">="))
                        caux->type = GE;
                    else if(!strcmp(pstr1,"<="))
                        caux->type = LE;
                    else if(!strcmp(pstr1,">"))
                        caux->type = GT;
                    else if(!strcmp(pstr1,"<"))
                        caux->type = LS;
                    else
                        goto syntax_error;

                    /* last field -- value */
                    pstr1 = NextField(&line);
                    if(!pstr1)
                        goto syntax_error;
                    caux->value = TransValue(pstr1);
                        
                    break;

                case 'W':
                    caux->topic = cWIDTH;

                    pstr1 = strchr(pstr,')');
                    if(!pstr1)
                        goto syntax_error;
                    *pstr1 = ' ';

                    pstr1 = strchr(pstr,'(');
                    if(!pstr1)
                        goto syntax_error;

                    sscanf(++pstr1, "%s",&name);

                    baux = pgFindBranchByName(ckt, name);
                    if(!baux){
                        sprintf(buf,
                        "Line: %d, cannot find branch: %s.",count, name);
                        error_mesg(PARSE_ERROR,buf);
                        continue;
                    }
                    /*printf("Width1 name: %s\n",name);*/

                    caux->bname1 = CopyStr(name);

                    pstr1 = NextField(&line);

                    if(!pstr1) 
                        goto syntax_error;

                    if(!strcmp(pstr1,"==")){

		        printf("EQ width constraint found.\n");

                        /* last field */
                        caux->type = EQ;
                        pstr = NextField(&line);
                        pstr1 = strchr(pstr,')');
                        if(!pstr1)
                            goto syntax_error;
                        *pstr1 = ' ';
                        pstr1 = strchr(pstr,'(');
                        if(!pstr1)
                            goto syntax_error;

                        sscanf(++pstr1, "%s",&name);

                        baux = pgFindBranchByName(ckt, name);
                        if(!baux){
                            sprintf(buf,
                            "Line: %d, cannot find branch: %s.",count, name);
                            error_mesg(PARSE_ERROR,buf);
                            continue;
                        }
                        /*printf("Width2 name: %s\n",name); */
                        caux->bname2 = CopyStr(name);
                    }
                    else if(!strcmp(pstr1,">") ||
                        !strcmp(pstr1,">=")){
                        /* last field */
                        caux->type = GE;
                        pstr = NextField(&line);
                        if(!pstr)
                            goto syntax_error;
                        caux->value = TransValue(pstr);
                    }
                    else {
                        sprintf(buf,"Line : %d, %s",
                        count, backbuf);
                        error_mesg(PARSE_ERROR, buf); 
                        continue;
                    }
                    break;

                default:
                    sprintf(buf, 
                    "Line: %d, unknow constrain type --> %s", count, backbuf);
                    error_mesg(PARSE_ERROR, buf);
                    break;

            }
        }
        else{
            while(*pstr == ' ') pstr++;
            switch(*pstr){
                case '\n':
                case '\0':
                case '*':
                case '#':
                continue;
                break;
                default:
                sprintf(buf, "Line: %d, %s", count, backbuf);
                error_mesg(PARSE_ERROR, buf);
            }
        }
            
    }
    /* obtain the layer information for each branch */
    pgFindLayerInfor(ckt);

    return;
    syntax_error:
        sprintf(buf, "Line: %d, %s", count, backbuf);
        error_mesg(PARSE_ERROR, buf);

}


/* build a new consraint structure and
   do the initialization.
*/
ExtConst *pgNewConst()
{
    ExtConst *caux;

    caux = (ExtConst *)malloc(sizeof(ExtConst));
    if(!caux){
        error_mesg(MEM_ERROR,"no memeory");
        return NULL;
    }
    caux->topic = cVOL;
    caux->type = ctUnknown;
    caux->bname1 = NULL;
    caux->bname2 = NULL;
    caux->node1 = 0;
    caux->value = 0.0;
    caux->next = NULL;
    return caux;
}

/* add a new constrain into the global constrain list.
*/

void pgAddExtConst(Circuit *ckt, ExtConst *econst )
{
    ExtConst  *caux;
    if(!econst)
        return;
    
    if(!ckt->theExtConstList)
        ckt->theExtConstList = econst;
    else{
        econst->next = ckt->theExtConstList;
        ckt->theExtConstList = econst;
    }
}


/*
**    Destroy the external constraint list
*/
void pgDestroyConst(ExtConst *ext_const)
{
    if(!ext_const)
        return;
    if(ext_const->next)
        pgDestroyConst(ext_const->next);
    if(ext_const->bname1)
        free(ext_const->bname1);
}
