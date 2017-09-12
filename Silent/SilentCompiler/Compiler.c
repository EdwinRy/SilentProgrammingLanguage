#include "Compiler.h"
void Compile(char * source)
{
	Program* program = malloc(sizeof(Program));
	program->tokensPointer = 0;
	program->bytecodePointer = 0;
	program->valuePointer = 0;
	program->sourceLength = strlen(source);
	program->source = source;
	program->tokens = malloc(program->sourceLength);
	program->values = malloc(sizeof(char*) * 200);
	//printf("%c",*(program->source+1));
	Tokenize(program);

	for (int i = 0; i <= program->tokensPointer; i++) 
	{
		printf("%i \n", *(program->tokens + 1));
		getchar();
	}
}

char** split(char* string, long long stringLength) 
{
	char** tokens = malloc(stringLength);
	long long numberOfTokens = 0;
	char* buffer = malloc(256);
	int bufferPointer = 0;

	//count tokens
	for (int i = 0; i < stringLength; i++) {
		if (string[i] == ' ' || string[i] == '\n')
			numberOfTokens++;
	}
}

void CompileAssembly(assembly* asm)
{
	char** tokens = split(asm->code, asm->codeSize);
	printf(asm->outputFile);

}