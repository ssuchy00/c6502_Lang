#pragma once
#include "Tokenizer.h"
#include "Stack.h"
#include "File.h"
#include "Symbol.h"


//SCREEN START $0200
//SCREEN END $05FF

typedef struct compiler_t {
	File* file;
	Tokenizer* tokenizer;
	Stack* memstack;
	Token* current;
	Token* prev;
	long stp;
} Compiler;

Compiler* Compiler_Create();
void Compiler_Init(Compiler* compiler, const char* filename);
void Compiler_Compile(Compiler* compiler);
void Compiler_Translate(Compiler* compiler);

int Compiler_HandleToken_def(Compiler* it, Token* token); 
int Compiler_HandleToken_sym(Compiler* it, Token* token);
int Compiler_HandleToken_eoi(Compiler* it, Token* token);
int Compiler_HandleToken_lit(Compiler* it, Token* token);
int Compiler_HandleToken_equ(Compiler* it, Token* token);
int Compiler_HandleToken_mop(Compiler* it, Token* token);

int Compiler_NullTest(Compiler* it);

long Compiler_AddSymbol(Compiler* it, Token* token);
long Compiler_FindSymbol(Compiler* it, Token* token);

 