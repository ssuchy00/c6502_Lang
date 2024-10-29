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

	while (compiler->tokenizer->tokens->current > 0)
	{
		test = Stack_pop(compiler->tokenizer->tokens);
		printf("[%d, \"%s\"]\n", test->token, test->content);
	}



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
		compiler->prev = Tokenizer_CreateToken(-1, "");
		switch (curr->token)
		{
		case TK_DEF: 
			ret = Compiler_HandleToken_def(compiler, curr);
			break;
		}

		if(ret>0)printf("--RET %d-- \n", ret);
	}

}

int Compiler_HandleToken_def(Compiler* it, Token* token)
{
	if (!Compiler_NullTest(it))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(it->tokenizer->tokens);

	switch (next->token)
	{
	case TK_SYM:
		it->prev = token;
		return Compiler_HandleToken_sym(it, next);
		break;
	default: 
		return 1;
	}

	return 0;
}
 

int Compiler_HandleToken_sym(Compiler* it, Token* token)
{
	if (!Compiler_NullTest(it))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(it->tokenizer->tokens);
	Token* prev = it->prev;
	long address = -1;

	//Check prev token
	switch (prev->token)
	{
	case TK_DEF: //prev
		address = Compiler_AddSymbol(it, token); //ADD ADDRESS TO STACK
		//Check next token
		switch (next->token)
		{
		case TK_EOI://->EOI: LDA 0, PHA
			printf("LDA 0\n");
			printf("PHA\n");
			return 0;
		case TK_EQU://->EQU: 
			it->prev = token;
			return Compiler_HandleToken_equ(it, next);
		default:
			return 1;
		}

		break;
	case TK_EQU: //prev
		address = Compiler_FindSymbol(it, token);//GET SYMBOL FROM STACK
		switch (next->token)
		{
		case TK_EOI://->EOI: LDA $X, PHA
			printf("LDA $%ld (%s)\n", address, token->content);
			printf("PHA\n");
			return 0;
		case TK_MOP://->MOP: --
			it->prev = token;
			return Compiler_HandleToken_mop(it, next);
		default:
			return 1;
		}

		break;
	case TK_MOP: //prev
		address = Compiler_FindSymbol(it, token);//GET SYMBOL FROM STACK
		if (strcmp("+", prev->content) == 0)
		{
			printf("ADC $%ld (%s)\n", address, token->content);
		}
		switch (next->token)
		{
		case TK_EOI://->EOI: PHA
			printf("PHA\n");
			return 0;
		case TK_MOP://->MOP:
			it->prev = token;
			return Compiler_HandleToken_mop(it, next);
		default:
			return 1;
		}
	
	default: 
		return 1;
	}
	 
	return 0;
}

int Compiler_HandleToken_eoi(Compiler* it, Token* token)
{
	if (!Compiler_NullTest(it))return 1;
	if (token == NULL)return 1;
	return 0;
}

int Compiler_HandleToken_lit(Compiler* it, Token* token)
{
	if (!Compiler_NullTest(it))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(it->tokenizer->tokens);
	Token* prev = it->prev;

	//Check prev token
	switch (prev->token)
	{
	case TK_EQU:	//prev
		//Check next token
		switch (next->token)
		{
		case TK_EOI://->EOI: LDA X, PHA
			printf("LDA %s\n", token->content);
			printf("PHA\n");
			return 0;
		case TK_MOP:
			printf("LDA %s\n", token->content);
			it->prev = token;
			return Compiler_HandleToken_mop(it, next);//->MOP: --	
		default:
			return 1;
		}
			
		return 0;
	case TK_MOP:	//prev
		if (strcmp("+", prev->content) == 0)
		{
			printf("ADC %s\n", token->content);
		}

		switch (next->token)
		{
		case TK_EOI://->EOI: PHA 
			printf("PHA\n");
			return 0;
		case TK_MOP:
			it->prev = token;
			return Compiler_HandleToken_mop(it, next);
		default: 
			return 1;
		}
	}
	 
	return 0;
}

int Compiler_HandleToken_equ(Compiler* it, Token* token)
{
	if (!Compiler_NullTest(it))return 1;
	if (token == NULL)return 1;

	Token* next = Stack_pop(it->tokenizer->tokens);
	switch (next->token)
	{
	case TK_LIT:
		it->prev = token;
		return Compiler_HandleToken_lit(it, next);
	case TK_SYM:
		it->prev = token;
		return Compiler_HandleToken_sym(it, next);
	default: 
		return 1;
	}

	return 0;
}

int Compiler_HandleToken_mop(Compiler* it, Token* token)
{
	if (!Compiler_NullTest(it))return 1;
	if (token == NULL)return 1;
	 
	Token* next = Stack_pop(it->tokenizer->tokens);
	switch (next->token)
	{
	case TK_LIT:
		it->prev = token;
		return Compiler_HandleToken_lit(it, next);
	case TK_SYM:
		it->prev = token;
		return Compiler_HandleToken_sym(it, next);
	default:
		return 1;
	}

	return 0;
}

int Compiler_NullTest(Compiler* it)
{
	if (it == NULL)return 0;
	if (it->tokenizer == NULL)return 0;
	if (it->tokenizer->tokens == NULL)return 0;
	return 1;
}

long Compiler_AddSymbol(Compiler* it, Token* token)
{
	return 0;
}

long Compiler_FindSymbol(Compiler* it, Token* token)
{
	return 0;
}
