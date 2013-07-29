/* double linked list
    rosettacode.org example
*/

struct List {
   struct MNode *head;
   struct MNode *tail;
   struct MNode *tail_pred;
};
 
struct MNode {
   struct MNode *succ;
   struct MNode *pred;
};
 
typedef struct MNode* NODE;
typedef struct List* LIST;
 
/*
** LIST l = newList()
** create (alloc space for) and initialize a list
*/
LIST newList(void);
 
/*
** int isEmpty(LIST l)
** test if a list is empty
*/
int isEmpty(LIST);
 
/*
** NODE n = getTail(LIST l)
** get the tail node of the list, without removing it
*/
NODE getTail(LIST);
 
/*
** NODE n = getHead(LIST l)
** get the head node of the list, without removing it
*/
NODE getHead(LIST);

NODE getNext(LIST l, NODE n);
 
/*
** NODE rn = addTail(LIST l, NODE n)
** add the node n to the tail of the list l, and return it (rn==n)
*/
NODE addTail(LIST, NODE);
 
/*
** NODE rn = addHead(LIST l, NODE n)
** add the node n to the head of the list l, and return it (rn==n)
*/
NODE addHead(LIST, NODE);
 
/*
** NODE n = remHead(LIST l)
** remove the head node of the list and return it
*/
NODE remHead(LIST);
 
/*
** NODE n = remTail(LIST l)
** remove the tail node of the list and return it
*/
NODE remTail(LIST);
 
/*
** NODE rn = insertAfter(LIST l, NODE r, NODE n)
** insert the node n after the node r, in the list l; return n (rn==n)
*/
NODE insertAfter(LIST, NODE, NODE);
 
/*
** NODE rn = removeNode(LIST l, NODE n)
** remove the node n (that must be in the list l) from the list and return it (rn==n)
*/
NODE removeNode(LIST, NODE);