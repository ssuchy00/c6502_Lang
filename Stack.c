#include "Stack.h"

Stack* Stack_create()
{
	Stack* stack = malloc(sizeof(Stack));
	if (stack == NULL)return NULL;
	stack->size = STACK_CHUNK;
	stack->current = 0;
	stack->elements = malloc(sizeof(void*) * stack->size);
	if (stack->elements == NULL)
	{
		free(stack);
		return NULL;
	}
	return stack;
}

void Stack_push(Stack* stack, void* element)
{
	if (stack == NULL || stack->elements == NULL || element == NULL)return;

	if (stack->current + 1 > stack->size)
	{
		void** elements = realloc(stack->elements, sizeof(void*) * (stack->size + STACK_CHUNK));
		if (elements == NULL)return;

		stack->size += STACK_CHUNK;
		stack->elements = elements;
	}

	stack->elements[stack->current++] = element;
}

void** Stack_pop(Stack* stack)
{
	if (stack == NULL || stack->elements == NULL || stack->current < 1)return NULL;  
	if (stack->size - stack->current >= 8)
	{
		void** elements = realloc(stack->elements, sizeof(void*) * (stack->size - STACK_CHUNK));
		if (elements == NULL)return;
		stack->size -= STACK_CHUNK;
		stack->elements = elements;
	}
	return stack->elements[--stack->current];
}