// ---------------- IMPORTATIONS ----------------

//standard
#include <stdio.h>
#include <stdlib.h>

//only for memcpy()
#include <string.h>

//own header
#include "queues.h"








/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Queues [0.1.1] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 Queues by I.A.

        Queues is an utility program that allows you to create and manipulate
    queues of any type (fixed type recommended).

        Theses queues are working as simple linked lists. Any kind of data
    can be stored inside them : different types / sizes / allocation places
    are allowed.

        ANY ITEM ADDED IS ALLOCATED INSIDE QUEUE IN HEAP. Make sure to use
    the correct queue_remove() or queue_pop() calls to free this data.

        As last point, some functions are using recursivity to work (for
    optimization). So there is ONE rule to keep in mind :

        ------------------------------------------
        /!\ DO NOT TURN A QUEUE INTO A CYCLE* ! /!\
        ------------------------------------------

    *(a cycle is a queue which has its last element pointing to its first)

    DISCLAIMER about mixed types :
            If you want to use different types in the same queue, you will
        have to remember the type each value refers to.
        (the first one is a float, the second one is an int, ...)

    22/01/2021 > [0.1.0] :
    - Created queues.c/.h.

    26/07/2023 > [0.1.1] :
    - Reformed the whole concept by adding queue_item structure.
      (Simplificated & optimized)

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************

    LICENCE :

    C_Queues
    Copyright (C) 2021 Sebastien SILVANO

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.

    If not, see <https://www.gnu.org/licenses/>.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */








// ---------------- INSTANTIATION ----------------

//create - clear
queue* queue_create(){ //data used will no longer stay inside queue, it will be copied

	//create queue
	queue* q = malloc(sizeof(queue));
	if(q == NULL){
		puts("FATAL ERROR > queues.c : queue_create() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//set initial values
	q->length = 0;
	q->first  = NULL;

	return q;
}

void queue_clear(queue* q){

	//error cases
	if(q == NULL){
		puts("RUNTIME ERROR > queues.c : queue_clear() : Queue is NULL.");
		return;
	}

	//already empty => do nothing (optimization)
	if(q->length == 0){ return; }

	//iterative remove (optimized compared to "for(a on length) remove(a)")
	queue_item* still_remaining = q->first;
	while(still_remaining != NULL){

		//keep a reference to the current element to remove
		queue_item* current = still_remaining;

		//keep also a reference to its (potentially) next remaining
		still_remaining = current->next;

		//free item
		free(current->data);
		free(current);
	}

	//reset length
	q->length = 0;
}








// ---------------- USE ----------------

//check integrity
static char queue_check(queue* q, unsigned int index){

	//NULL queue
	if(q == NULL){
		puts("RUNTIME ERROR > queues.c : queue_check() : Queue is NULL.");
		return 1;
	}

	//out of bound
	if(index >= q->length){
		printf("RUNTIME ERROR > queues.c : queue_check() : Index '%u' is out of bound ('%u' elements).\n", index, q->length);
		return 1;
	}

	return 0;
}

//get - set
static queue_item* queue_getItem(queue* q, unsigned int index){
	if(queue_check(q, index)){
		return NULL;
	}

	//iterate until the correct q is found
	queue_item* current = q->first;
	for(unsigned int ui=0; ui < index; ui++){

		//internal error (optional)
		#ifdef INTERNAL_ERRORS
		if(current == NULL){
			puts("INTERNAL ERROR : queues.c : queue_get() : Next element should exist but is NULL.\n");
			return NULL;
		}
		#endif

		current = current->next;
	}

	return current;
}

void* queue_get(queue* q, unsigned int index){
	if(queue_check(q, index)){
		return NULL;
	}

	//get queue item
	queue_item* qi = queue_getItem(q, index);

	//internal error (optional)
	#ifdef INTERNAL_ERRORS
	if(qi == NULL){
		printf("INTERNAL ERROR > queues.c : queue_get() : Could not found queue_item at index %u.\n", index);
		return NULL;
	}
	#endif

	//return its data
	return qi->data;
}

void queue_set(queue* q, unsigned int index, void* data, unsigned int size){
	if(queue_check(q, index)){
		return;
	}

	//incorrect given data
	if(data == NULL){
		puts("RUNTIME ERROR > queues.c : queue_set() : Data is NULL.");
		return;
	}
	if(size == 0){
		puts("RUNTIME ERROR > queues.c : queue_set() : Data size is 0.");
		return;
	}

	//get concerned element
	queue_item* qi = queue_getItem(q, index);

	//internal errors (optional)
	#ifdef INTERNAL_ERRORS
	if(qi == NULL){
		printf("INTERNAL ERROR > queues.c : queue_set() : Could not found queue_item at index %u.\n", index);
		return;
	}
	if(qi->data == NULL){
		printf("INTERNAL ERROR > queues.c : queue_set() : Data at specified index %u is NULL.\n", index);
		return;
	}
	#endif

	//free previous element
	free(qi->data);

	//create new data
	qi->data = malloc(size);
	if(qi->data == NULL){
		puts("FATAL ERROR > queues.c : queue_set() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//copy data into new location
	memcpy(qi->data, data, size);
}




//add - remove
void queue_insertAfter(queue* q, unsigned int index, void* data, unsigned int size){
	if(queue_check(q, index)){
		return;
	}

	//incorrect given data
	if(data == NULL){
		puts("RUNTIME ERROR > queues.c : queue_insertAfter() : Data is NULL.");
		return;
	}
	if(size == 0){
		puts("RUNTIME ERROR > queues.c : queue_insertAfter() : Data size is 0.");
		return;
	}

	//get concerned element
	queue_item* qi = queue_getItem(q, index);

	//internal errors (optional)
	#ifdef INTERNAL_ERRORS
	if(qi == NULL){
		printf("INTERNAL ERROR > queues.c : queue_insertAfter() : Could not found queue_item at index %u.\n", index);
		return;
	}
	if(qi->data == NULL){
		printf("INTERNAL ERROR > queues.c : queue_insertAfter() : Data at specified index %u is NULL.\n", index);
		return;
	}
	#endif

	//get the element in position index+1
	queue_item* qi_after = qi->next;

	//create new item
	qi->next = malloc(sizeof(queue_item));
	if(qi->next == NULL){
		puts("FATAL ERROR > queues.c : queue_insertAfter() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//create its data
	(qi->next)->data = malloc(size);
	if((qi->next)->data == NULL){
		puts("FATAL ERROR > queues.c : queue_insertAfter() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//copy data into new location
	memcpy((qi->next)->data, data, size);

	//attach the rest of the queue (that can be NULL)
	(qi->next)->next = qi_after;

	//increase length
	q->length++;
}

void queue_append(queue* q, void* data, unsigned int size){
	if(q == NULL){
		puts("RUNTIME ERROR > queues.c : queue_append() : Queue is NULL.");
		return;
	}

	//incorrect given data
	if(data == NULL){
		puts("RUNTIME ERROR > queues.c : queue_append() : Data is NULL.");
		return;
	}
	if(size == 0){
		puts("RUNTIME ERROR > queues.c : queue_append() : Data size is 0.");
		return;
	}

	//create new item
	queue_item* qi_new = malloc(sizeof(queue_item));
	if(qi_new == NULL){
		puts("FATAL ERROR > queues.c : queue_append() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//create its data
	qi_new->data = malloc(size);
	if(qi_new->data == NULL){
		puts("FATAL ERROR > queues.c : queue_append() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//copy data into new location
	memcpy(qi_new->data, data, size);

	//attach next element (no element)
	qi_new->next = NULL;

	//attach to existing queue : case 1 : empty queue
	if(q->first == NULL){
		q->first = qi_new;
	}

	//attach to existing queue : case 2 : get the latest element
	else{
		queue_item* last = q->first;
		while(last->next != NULL){ last = last->next; }
		last->next = qi_new;
	}

	//increase length
	q->length++;
}

void* queue_pop(queue* q, unsigned int index){
	if(queue_check(q, index)){
		return NULL;
	}
	queue_item* qi_toRemove;

	//remove from chain : case 1 : first element
	if(index == 0){
		qi_toRemove = q->first;
		q->first    = qi_toRemove->next;
	}

	//remove from chain : case 2 : get item preceding the one to remove
	else{
		queue_item* qi_before = queue_getItem(q, index-1);

		//internal error (optional)
		#ifdef INTERNAL_ERRORS
		if(qi_before == NULL){
			printf("INTERNAL ERROR > queues.c : queue_pop() : Could not found queue_item at index %u.\n", index-1);
			return;
		}
		#endif

		//new linking
		qi_toRemove     = qi_before->next;
		qi_before->next = qi_toRemove->next;
	}

	//free targetted qi
	void* contained_data = qi_toRemove->data;
	free(qi_toRemove);

	//decrease length
	q->length--;

	return contained_data;
}

void queue_remove(queue* q, unsigned int index){
	void* data = queue_pop(q, index);
	if(data != NULL){ free(data); }
}
