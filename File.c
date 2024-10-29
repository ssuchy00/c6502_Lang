#include "File.h"
#pragma warning(disable: 4996)

File* File_Open(const char* name, int mode)
{
	if (name == NULL)return NULL;
	if (mode < 0 || mode > __MODE_MAX__)return NULL;

	FILE* file = NULL;

	switch (mode)
	{
	case 0: 
		file = fopen(name, "rb");
		break;
	case 1:
		file = fopen(name, "wb");
		break;
	}

	if (file == NULL)return NULL;

	File* file_t = malloc(sizeof(File));
	if(file_t ==NULL)
	{
		fclose(file);
		return NULL;
	}

	file_t->fp = file;

	int len = strlen(name);

	file_t->name = malloc(len + 1);
	if (file_t->name == NULL)
	{
		free(file_t);
		fclose(file);
		return NULL;
	}

	strncpy(file_t->name, name, len); 
	file_t->name[len] = '\x0';
	file_t->content = NULL;
		
	return file_t;
}

void File_Read(File* file)
{
	if (file == NULL)return;
	if (file->fp == NULL)return;

	long fsize = File_getSize(file);

	file->content = malloc(fsize + 1);
	if (file->content == NULL)return NULL;

	fread(file->content, 1, fsize, file->fp);
	file->content[fsize] = '\x0';
}

void File_Free(File** file)
{
	if (file==NULL || *file == NULL)return;
	if ((*file)->name != NULL)free((*file)->name);
	if ((*file)->content != NULL)free((*file)->content);
	if ((*file)->fp != NULL)fclose((*file)->fp);
	free(*file);
	*file = NULL;
}

long File_getSize(File* file)
{
	if (file == NULL)return 0;
	if (file->fp == NULL)return 0;

	long ret = 0;

	fseek(file->fp, 0, SEEK_END);
	ret = ftell(file->fp);
	fseek(file->fp, 0, SEEK_SET);

	return ret;
}
