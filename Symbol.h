#pragma once
#pragma warning(disable:4996)

#include <stdlib.h>
#include <string.h>

typedef struct symbol_t {
	char* symbol;
	long address;
} Symbol;

Symbol* Symbol_create(char* symbol, long address);