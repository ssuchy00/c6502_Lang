#pragma once
#include <stdlib.h>

#define STACK_CHUNK 8

typedef struct stack_t {
	int current;
	int size;
	void** elements;
} Stack;

Stack* Stack_create();
void Stack_push(Stack* stack, void* element);
void** Stack_pop(Stack* stack); 
