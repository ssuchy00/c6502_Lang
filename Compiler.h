#pragma once
#include "Tokenizer.h"
#include "Stack.h"
#include "File.h"

typedef struct compiler_t {
	File* file;
	Tokenizer* tokenizer;
	Stack* memstack;
	Token* current;
	Token* prev;
} Compiler;

Compiler* Compiler_Create();
void Compiler_Init(Compiler* compiler, const char* filename);
void Compiler_Compile(Compiler* compiler);
void Compiler_Translate(Compiler* compiler);

int Compiler_HandleToken_def(Compiler* compiler, Token* token);
int Compiler_HandleToken_defsym(Compiler* compiler, Token* token);
int Compiler_HandleToken_sym(Compiler* compiler, Token* token);
int Compiler_HandleToken_eoi(Compiler* compiler, Token* token);
int Compiler_HandleToken_lit(Compiler* compiler, Token* token);
int Compiler_HandleToken_equ(Compiler* compiler, Token* token);
int Compiler_HandleToken_mop(Compiler* compiler, Token* token);

int Compiler_NullTest(Compiler* compiler);

long Compiler_AddSymbol(Compiler* compiler, Token* token);
long Compiler_FindSymbol(Compiler* compiler, Token* token);

 