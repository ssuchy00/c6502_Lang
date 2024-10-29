#include "Compiler.h"

Compiler* Compiler_Create()
{
	Compiler* compiler = malloc(sizeof(Compiler));
	if (compiler == NULL)return NULL;

	compiler->file = NULL;
	compiler->memstack = NULL;
	compiler->current = NULL;
	compiler->prev = NULL;
	compiler->tokenizer = Tokenizer_Create();
	if (compiler->tokenizer == NULL)
	{
		free(compiler);
		return NULL;
	}

	compiler->memstack = Stack_create();
	if (compiler->memstack == NULL)
	{
		File_Free(compiler->file);
		free(compiler->tokenizer);
		free(compiler);
		return NULL;
	}

	return compiler;
}

void Compiler_Init(Compiler* compiler, const char* filename)
{
	if (compiler == NULL)return;
	compiler->file = File_Open(filename, MODE_READ);
	if (compiler->file == NULL) return;
	
	File_Read(compiler->file);
	Tokenizer_ReadContent(compiler->tokenizer, compiler->file);
	File_Free(&compiler->file);
}

void Compiler_Compile(Compiler* compiler)
{
	if (compiler == NULL)return;
	if (compiler->tokenizer == NULL)return;
	if (compiler->tokenizer->content == NULL)return;
	 
	Tokenizer_Tokenize(compiler->tokenizer);

	Token* test = NULL; 
	Stack* new_tokens = Stack_create();
	while(compiler->tokenizer->tokens->current > 0)
	{ 
		Stack_push(new_tokens, Stack_pop(compiler->tokenizer->tokens));
	}

	compiler->tokenizer->tokens = new_tokens;

	Compiler_Translate(compiler);

	//while (compiler->tokenizer->tokens->current > 0)
	//{
	//	test = Stack_pop(compiler->tokenizer->tokens);
	//	printf("%d %s\n", test->token, test->content);
	//}



}

void Compiler_Translate(Compiler* compiler)
{
	if (compiler == NULL)return;
	if (compiler->tokenizer == NULL)return;
	if (compiler->tokenizer->tokens == NULL)return;

	Token* curr = NULL; 
	int ret = 0;
	while (compiler->tokenizer->tokens->current > 0)
	{
		curr = Stack_pop(compiler->tokenizer->tokens);
		compiler->current = curr;
		compiler->prev = NULL;
		switch (curr->token)
		{
		case TK_DEF: 
			ret = Compiler_HandleToken_def(compiler, curr);
			break;
		}

		//printf("--RET %d-- \n", ret);
	}

}

int Compiler_HandleToken_def(Compiler* compiler, Token* token)
{
	if (!Compiler_NullTest(compiler))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(compiler->tokenizer->tokens);
	switch (next->token)
	{
	case TK_SYM:
		compiler->prev = token;
		return Compiler_HandleToken_sym(compiler, next);
		break;
	default:
		return 1;
	}
	return 0;
}

int Compiler_HandleToken_defsym(Compiler* compiler, Token* token)
{
	
}

int Compiler_HandleToken_sym(Compiler* compiler, Token* token)
{
	if (!Compiler_NullTest(compiler))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(compiler->tokenizer->tokens);

	if (compiler->prev->token == TK_MOP)
	{
		if (strcmp(compiler->prev->content, "+") == 0)
		{
			long address = -1;
			address = Compiler_FindSymbol(compiler, token);
			printf("ADC $%ld (%s)\n", address, token->content);
		}
	}

	if (compiler->prev->token == TK_EQU)
	{
		long address = -1;
		address = Compiler_FindSymbol(compiler, token);
		printf("LDA $%ld (%s)\n", address, token->content);
	}

	switch (next->token)
	{
	case TK_EQU: {
		long address = -1;
		if (compiler->prev == TK_DEF)
			address = Compiler_AddSymbol(compiler, token);
		else if (compiler->prev == NULL)
			address = Compiler_FindSymbol(compiler, token);
		compiler->prev = token;
		return Compiler_HandleToken_equ(compiler, next);
		break;
	}
	case TK_MOP: {
		compiler->prev = token;
		return Compiler_HandleToken_mop(compiler, next);
	}
	case TK_EOI:
		printf("PHA\n");
		compiler->prev = token;
		return Compiler_HandleToken_eoi(compiler, next);
		break;
	default:
		return 1;
	}
	return 0;
}

int Compiler_HandleToken_eoi(Compiler* compiler, Token* token)
{
	if (!Compiler_NullTest(compiler))return 1;
	if (token == NULL)return 1;
	return 0;
}

int Compiler_HandleToken_lit(Compiler* compiler, Token* token)
{
	if (!Compiler_NullTest(compiler))return 1;
	if (token == NULL)return 1;
	Token* next = Stack_pop(compiler->tokenizer->tokens);
	switch (next->token)
	{
	case TK_EOI: {
		if (compiler->prev->token == TK_EQU)
		{
			printf("LDA %s\n", token->content);
		}
		else if(compiler->prev->token == TK_MOP) {
			if (strcmp(compiler->prev->content, "+") == 0)
			{
				printf("ADC %s\n", token->content);
			}
		}
		else {
			return 1;
		}
		printf("PHA\n");

		return 0;

	}
	default:
		return 1;
	}
	return 0;
}

int Compiler_HandleToken_equ(Compiler* compiler, Token* token)
{
	if (!Compiler_NullTest(compiler))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(compiler->tokenizer->tokens);
	switch (next->token)
	{
	case TK_SYM:
		compiler->prev = token;
		return Compiler_HandleToken_sym(compiler, next);
		break;
	case TK_LIT:
		compiler->prev = token;
		return Compiler_HandleToken_lit(compiler, next);
		break;
	 
	default:
		return 1;
	}
	return 0;
}

int Compiler_HandleToken_mop(Compiler* compiler, Token* token)
{
	if (!Compiler_NullTest(compiler))return 1;
	if (token == NULL)return 1;
	Token* next = Stack_pop(compiler->tokenizer->tokens);
	switch (next->token)
	{
	case TK_SYM: {
		compiler->prev = token;
		return Compiler_HandleToken_sym(compiler, next);
	}
	case TK_LIT: {
		compiler->prev = token;
		return Compiler_HandleToken_lit(compiler, next);
	}
	default:
		return 1;
	}
	return 0;
}

int Compiler_NullTest(Compiler* compiler)
{
	if (compiler == NULL)return 0;
	if (compiler->tokenizer == NULL)return 0;
	if (compiler->tokenizer->tokens == NULL)return 0;
	return 1;
}

long Compiler_AddSymbol(Compiler* compiler, Token* token)
{
	return 0;
}

long Compiler_FindSymbol(Compiler* compiler, Token* token)
{
	return 0;
}
