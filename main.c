#pragma warning(disable: 4996)

#include <stdio.h>
#include "Compiler.h"

int main()
{
	Compiler* compiler = Compiler_Create();
	Compiler_Init(compiler, "Test/test.txt");
	Compiler_Compile(compiler);

	return 0;
}