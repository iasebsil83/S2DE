#ifndef PROCESSES_H
#define PROCESSES_H








// -------------------------------- DEFINITIONS --------------------------------

//constants
#define PROC__STOP_KILL 0
#define PROC__STOP_WAIT 1

//structure
struct PROC{
	char* exePath;
	char** args;
	pid_t pid;
};
typedef struct PROC proc;








// -------------------------------- PROCESSES --------------------------------

//create - delete
proc* proc_create(char* exePath, char** args);
void proc_delete(proc* p); //will free #args# but not #exePath# ! (allowing you to use a string litteral for #exePath#)

//start - stop
void proc_start(proc* p);
void proc_stop(proc* p, char mode);








#endif
