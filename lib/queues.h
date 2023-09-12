#ifndef QUEUES_H
#define QUEUES_H






// ---------------- DEFINITIONS ----------------

//structures
typedef struct QUEUE_ITEM {
	struct QUEUE_ITEM* next;
	void*              data;
} queue_item;
typedef struct QUEUE {
	unsigned int length;
	queue_item*  first;
} queue;






// ---------------- INSTANTIATION ----------------

//create - clear
queue* queue_create();
void   queue_clear(queue* q);






// ---------------- USE ----------------

//get - set
void* queue_get(queue* q, unsigned int index);
void  queue_set(queue* q, unsigned int index, void* data, unsigned int size);

//add - remove
void  queue_insertAfter(queue* q, unsigned int index, void* data, unsigned int size);
void  queue_append(     queue* q,                     void* data, unsigned int size);
void* queue_pop(        queue* q, unsigned int index);
void  queue_remove(     queue* q, unsigned int index);






#endif
