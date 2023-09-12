// ---------------- IMPORTATIONS ----------------

//standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//threads
#include "../lib/threads.h"

//bipipes
#include "../lib/bipipes.h"






// ---------------- DECLARATIONS ----------------

//global variables
#define GET false
#define SET true






// ---------------- GETTERS - SETTERS ----------------

//global variables
bipipe* comBipipe(bool action, bipipe* value){ //communication bipipe
	static bipipe* com_bipipe = NULL;
	if(action){ com_bipipe = value; } //set
	return com_bipipe;                //get
}






// ---------------- EXECUTION ----------------

//events
void* manageEvents(void* args){

	//get communication bipipe
	bipipe* com_bipipe = comBipipe(GET, NULL);

	//receiving events indefinitely
	while(1){
		char* events_text = bipipe_read(com_bipipe);
		printf("Received event text \"%s\".\n", events_text);
		//usleep(1000000);
		free(events_text);
	}
}



//main
int main(int argc, char** argv){

	//debug
	printf("argc : [%i]\n\n", argc);
	for(unsigned int a=0; a < argc; a++){ printf("argv[%i] : [%s]\n", a, argv[a]); }
	putc('\n', stdout);



	// BIPIPE CONNECTION

	//missing bipipe argument
	if(argc == 1){
		fputs("FATAL_ERROR > techt.c : main() : Missing bipipe info at first position in peer subprocess.\n", stderr);
		return EXIT_FAILURE;
	}

	//connect bipipe as we are a CHILD process
	bipipe* com_bipipe = comBipipe(SET, bipipe_join(argv[1])); //get & set at the same time
	//usleep(1000000);

	//event listening thread
	thread* events_thread = thread_create(manageEvents);
	thread_start(events_thread);



	// EXECUTION

	//send orders to S2DE
	bipipe_write(com_bipipe, "i00000438000001e0my Wonderful window`");

	//just a waiting loop
	while(1){
		usleep(100);
	}

	return EXIT_SUCCESS;
}
