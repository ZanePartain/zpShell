#include "queue.c"
struct proc proc[NPROC];      // NPROC PROCs
struct proc *freeList;        // freeList of PROCs 
struct proc *readyQueue;      // priority queue of READY procs
struct proc *running;         // current running proc pointer

struct proc *sleepList;       // list of SLEEP procs
int tswitch();

int sleep(int event)
{
  printf("proc %d going to sleep on event=%d\n", running->pid, event);

  running->event = event;
  running->status = SLEEP;
  enqueue(&sleepList, running);
  printList("sleepList", sleepList);
  tswitch();
}

int wakeup(int event)
{
  struct proc *temp, *p;
  temp = 0;
  printList("sleepList", sleepList);

  while (p = dequeue(&sleepList)){
     if (p->event == event){
        printf("wakeup %d\n", p->pid);
        p->status = READY;
        enqueue(&readyQueue, p);
     }
     else{
	      enqueue(&temp, p);
     }
  }
  sleepList = temp;
  printList("sleepList", sleepList);
}

int kexit(int exitValue)
{
  //is INIT process P1
  if(exitValue == 1){
    // if P1 has no children then it can die.
  }
  else{
    printf("proc %d in kexit(), value=%d\n", running->pid, exitValue);
    running->exitCode = exitValue;          //set exit value of running proc
    running->status = ZOMBIE;               //kill the proc
    
    if(running->child){
      struct proc *rchild = running->child; //set ptr to running proc child
      struct proc *p1child = INIT->child;   //set ptr to p1 child
      if(p1child){
        while(p1child->sibling){            //go to end of p1 child list
          p1child = p1child->sibling; 
        }

        p1child->sibling = rchild;          //attach entire rchild list to end of p1child list

        while(rchild){                      //set rchild parent ptr to p1
          rchild->ppid = INIT->pid;
          rchild->parent = INIT;
          rchild = rchild->sibling;
        }
        running->child = NULL;
      }
      else{
        //no running children of P1
      }
    }
    wakeup(running->ppid);         //wake up parent if under sleep
    tswitch();
  }
}

int _DeconstructPROC(struct proc *ptrPROC){
  ptrPROC->sibling = NULL;   //isolate proc
  ptrPROC->parent = NULL;    //remove proc parent
  ptrPROC->ppid = 0;         
  ptrPROC->status = FREE;    //set ZOMB child status to FREE
}

int RemoveChild(struct proc *ptrChild, struct proc *ptrPrevChild){
   if(!ptrChild->sibling){                        //only child is ZOMB of running proc
      running->child = NULL;
    }
    else if(running->child == ptrChild){          //if ZOMB is at front of running proc child list
      running->child = ptrChild->sibling;         //set running proc child ptr to ZOMB sibling
    }
    else{                                         //more than one child of running proc
      ptrPrevChild->sibling = ptrChild->sibling;  //unlink ZOMB from running child list
    }
}

int wait(int *status){
  int zomBid;

  if(running->child){
    struct proc *child = running->child;
    struct proc *prev = running;
    //infinite loop over the children of the current waiting process
    // until the child proccess has finished
    while(1){
      if(child->status == ZOMBIE){
        zomBid = child->pid;            //get ZOMB pid
        (*status) = child->exitCode;    //copy ZOMB exit code
        RemoveChild(child, prev);       //unlink ZOMB child from running proc child list
        _DeconstructPROC(child);        //isolate and reset ZOMB child
        enqueue(&freeList,child);       //put ZOMB in freeList
        wakeup(running->ppid);          //wakeup running proc parent
        return zomBid;
      } 
      if(!child->sibling){              //if the next sibling is null
        child = running->child;         //set child to start of running proc child list
      }
      else{                             //child sibling != NULL
        prev = child;                       
        child = child->sibling;
      }
      sleep(running->pid);
    }
  }
  else{
    return -1; //error code
  }      
}
  
