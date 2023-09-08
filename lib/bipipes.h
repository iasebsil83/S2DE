#ifndef BIPIPES_H
#define BIPIPES_H








// -------------------------------- DEFINITIONS --------------------------------

//structure
struct BIPIPE{
	int* pRcW_fd; //parent READ  - child WRITE
	int* pWcR_fd; //parent WRITE - child READ
	char* info;
	char isParent;
	char ready;
	unsigned int length;
};
typedef struct BIPIPE bipipe;








// -------------------------------- BIPIPES --------------------------------

//create - delete
bipipe* bipipe_create(unsigned int length);
void bipipe_delete(bipipe* bp);

//join
bipipe* bipipe_join(char* info);
void bipipe_everybodyJoined(bipipe* bp);

//read - write
char* bipipe_read(bipipe* bp);
void bipipe_write(bipipe* bp, char* data); // data will not be free after use
                                           // WARNING ! data must have length >= bp->length ('\0' IS NOT DETERMINATIVE)







#endif
