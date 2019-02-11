#include "type.h"

/***************** queue.c file *****************/
int enqueue(struct proc **queue, struct proc *p) 
{
  struct proc *q = *queue;
  if (q == 0 || p->priority > q->priority){
     *queue = p;
     p->next = q;
  }
  else{
     while (q->next && p->priority <= q->next->priority)
            q = q->next;
     p->next = q->next;
     q->next = p;
  }
}
struct proc *dequeue(struct proc **queue) 
{
     struct proc *p = *queue;
     if (p)
        *queue = (*queue)->next;
     return p;
}
int printList(char *name, struct proc *p) 
{
  printf("%s = ", name);
  while(p){
     printf("[%d %d]->", p->pid, p->priority);
     p = p->next;
  }
  printf("NULL\n");
}

int printSleep(char *name, struct proc *p) 
{
  printf("%s = ", name);
  while(p){
     printf("[%d event=%d]->", p->pid, p->event);
     p = p->next;
  }
  printf("NULL\n");
}

int printChildren(char *words, struct proc *child)
{
   printf("%s", words);
   while(child){
      printf("->[%d]-", child->pid);
      child = child->sibling;
   }
   putchar('\n');
}

