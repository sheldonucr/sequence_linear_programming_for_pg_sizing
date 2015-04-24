#include "lpkit.h"
#include <stdio.h>
#include <assert.h>


typedef struct _lp_queue_elem
{
    matrec *elem;
    struct _lp_queue_elem *next;
} lpQueueElem;

static lpQueueElem *head, *tail;

void
lp_clean_queue()
{
    lpQueueElem *maux;
    while(head){ 
        maux = head->next; 
	free(head); 
	head = maux;
    }
    head = NULL;
    tail = NULL;
}
    
void
lp_enqueue( matrec *elem)
{
    lpQueueElem *aux = malloc(sizeof(lpQueueElem));
    assert(aux);
    aux->next = NULL;
    aux->elem = elem;
    if(!tail){
        head = tail = aux;    
	return;
    }
    else{
        tail->next = aux; 
	tail = aux;
    }
}

matrec * 
lp_dequeue()
{
    lpQueueElem *aux;
    matrec *elem;

    if(!head)
        return NULL;
    else{
       aux = head;
       head = head->next;
       if(!head)
           tail = NULL;
    }
    elem = aux->elem;
    free(aux);
    return elem;
}

void 
get_column_in_queueu(
    lprec *lp, 
    int col_nr)
{
  int i;

  lp_clean_queue();
  if(col_nr < 1 || col_nr > lp->columns)
    error("Col. nr. out of range in get_column");
  for(i = lp->col_end[col_nr-1]; i < lp->col_end[col_nr]; i++){
    lp->mat[i].scaled_value = lp->mat[i].value;
    if(lp->ch_sign[i])
          lp->mat[i].scaled_value *= -1;
    if(lp->scaling_used)
          lp->mat[i].scaled_value /= 
		(lp->scale[i] * lp->scale[lp->rows + col_nr]);
    
  lp_enqueue(&(lp->mat[i]));
  }
}
