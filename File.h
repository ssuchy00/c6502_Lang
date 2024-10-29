#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MODE_READ 0
#define MODE_WRITE 1
#define __MODE_MAX__ 1


typedef struct file_t {
	char* name;
	FILE* fp;
	char* content;
} File;

File* File_Open(const char* name, int mode);
void File_Read(File* file);
void File_Free(File** file);
long File_getSize(File* file);