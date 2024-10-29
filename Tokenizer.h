#pragma once
#include <string.h>
#include "Stack.h"
#include "File.h"
#include "tokens.h"
#include "_keywords.h"

#pragma warning(disable: 4996)

typedef struct tokenizer_t {
	char* content;
	Stack* tokens;
} Tokenizer;

typedef struct token_t {
	int token;
	char* content;
} Token;

Tokenizer* Tokenizer_Create();
void Tokenizer_ReadContent(Tokenizer* tokenizer, File* file);
void Tokenizer_Tokenize(Tokenizer* tokenizer);

Token* Tokenizer_CreateToken(int token, const char* content);
