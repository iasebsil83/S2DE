#ifndef ________PNG_H
#define ________PNG




















// ---------------- IMPORTATIONS ----------------

//structure
typedef struct{
	int width, height;
	int* data;	
} PNG;




















// ---------------- BASICS ----------------

//read - write
PNG* png_read(char* fileName);
void png_write(PNG* image, char* fileName);




















#endif
