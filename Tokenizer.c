#include "Tokenizer.h"

Tokenizer* Tokenizer_Create()
{
	Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
	if (tokenizer == NULL)return NULL;
	tokenizer->content = NULL;
	tokenizer->tokens = Stack_create();
	if (tokenizer->tokens == NULL)
	{
		free(tokenizer);
		return NULL;
	}

	return tokenizer;
}

void Tokenizer_ReadContent(Tokenizer* tokenizer, File* file)
{
	if (tokenizer == NULL)return;
	if (file == NULL)return;
	if (file->content == NULL)return;

	long len = strlen(file->content);
	tokenizer->content = malloc(len + 1);
	if (tokenizer->content == NULL)return;
	strncpy(tokenizer->content, file->content, len);
	tokenizer->content[len] = '\x0';
}

void Tokenizer_Tokenize(Tokenizer* tokenizer)
{
	if (tokenizer == NULL)return;
	if (tokenizer->content == NULL || tokenizer->tokens==NULL)return;

	char curr[255];
	int curr_index = 0;
	int i = 0;
	char* content = tokenizer->content;
	char c;
	while (content[i] != '\x0')
	{
		c = content[i];
		if (isalpha(c))
		{
			while (isalnum(c))
			{
				c = content[i++];
				curr[curr_index++] = c;
			}
			//curr_index--;
			i-=2;
			curr_index -= (curr_index > 1 ? 1 : 0);
			curr[curr_index] = '\x0';
			curr_index = 0;

			Token* tk = NULL;

			if (strcmp(curr, KW_DEF) == 0)tk = Tokenizer_CreateToken(TK_DEF, ""); 
			else tk = Tokenizer_CreateToken(TK_SYM, curr);

			Stack_push(tokenizer->tokens, tk);
			if (tk->token == TK_EOI)return; 
			int x = 4;
		}

		else if (isdigit(c))
		{
			while (isdigit(c))
			{
				c = content[i++];
				curr[curr_index++] = c;
			}
			i-=2;
			curr_index -= (curr_index > 0 ? 1 : 0);
			curr[curr_index] = '\x0';
			curr_index = 0;

			Token* tk = NULL;

			tk = Tokenizer_CreateToken(TK_LIT, curr);

			Stack_push(tokenizer->tokens, tk);
			if (tk->token == TK_EOI)return;
			int x = 4;
		}
		else if (c == '=')
		{
			Token* tk = Tokenizer_CreateToken(TK_EQU, "");
			Stack_push(tokenizer->tokens, tk);
			int x = 4;
		}
		else if (c == '+')
		{
			Token* tk = Tokenizer_CreateToken(TK_MOP, "+");
			Stack_push(tokenizer->tokens, tk);
			int x = 4;
		}
		else if (c == ';')
		{
			Token* tk = Tokenizer_CreateToken(TK_EOI, "");
			Stack_push(tokenizer->tokens, tk);
			//return;
			int x = 4;
		}

		i++;
 
	}

	
}

Token* Tokenizer_CreateToken(int token, const char* content)
{
	Token* tk = malloc(sizeof(Token));
	if (tk == NULL)return;
	long len = strlen(content);
	tk->content = malloc(len + 1);
	if (tk->content == NULL)
	{
		free(tk);
		return NULL;
	}
	strncpy(tk->content, content, len);
	tk->content[len] = '\x0';

	tk->token = token;
	return tk;
}
