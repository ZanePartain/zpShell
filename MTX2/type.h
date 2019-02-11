/*********** type.h file ************/
#define NPROC    9          // number of PROCs
#define SSIZE 1024          // stack size = 4KB

// struct proc status
#define FREE     0          
#define READY    1
#define SLEEP    2
#define ZOMBIE   3

struct proc{
    struct proc *next;      // next proc pointer       
    int  *ksp;              // saved sp: at byte offset 4 

    int   pid;              // process ID
    int   ppid;             // parent process pid 
    int   status;           // struct proc status=FREE|READY, etc. 
    int   priority;         // scheduling priority 

    int   event;            // event value to sleep on
    int   exitCode;         // exit value

    struct proc *child;     // first child struct proc pointer       
    struct proc *sibling;   // sibling struct proc pointer  
    struct proc *parent;    // parent struct proc pointer       

    int   kstack[1024];     // process stack                 
};

struct proc *INIT;          // to always point at P1