/* double linked list
    rosettacode.org example
*/

#include "djikstra_maps.h"

LIST newList(void)
{
    LIST tl = malloc(sizeof(struct List));
    if ( tl != NULL )
    {
       tl->tail_pred = (NODE)&tl->head;
       tl->tail = NULL;
       tl->head = (NODE)&tl->tail;
       return tl;
    }
    return NULL;
}
 
int isEmpty(LIST l)
{
   return (l->head->succ == 0);
}
 
NODE getHead(LIST l)
{
  return l->head;
}
 
NODE getTail(LIST l)
{
  return l->tail_pred;
}

NODE getNext(LIST l, NODE n)
{
  return n->succ;
}
 
 
NODE addTail(LIST l, NODE n)
{
    n->succ = (NODE)&l->tail;
    n->pred = l->tail_pred;
    l->tail_pred->succ = n;
    l->tail_pred = n;
    return n;
}
 
NODE addHead(LIST l, NODE n)
{
    n->succ = l->head;
    n->pred = (NODE)&l->head;
    l->head->pred = n;
    l->head = n;
    return n;
}
 
NODE remHead(LIST l)
{
   NODE h;
   h = l->head;
   l->head = l->head->succ;
   l->head->pred = (NODE)&l->head;
   return h;
}
 
NODE remTail(LIST l)
{
   NODE t;
   t = l->tail_pred;
   l->tail_pred = l->tail_pred->pred;
   l->tail_pred->succ = (NODE)&l->tail;
   return t;
}
 
NODE insertAfter(LIST l, NODE r, NODE n)
{
   n->pred = r; n->succ = r->succ;
   n->succ->pred = n; r->succ = n;
   return n;
}
 
NODE removeNode(LIST l, NODE n)
{
   n->pred->succ = n->succ;
   n->succ->pred = n->pred;
   return n;
}