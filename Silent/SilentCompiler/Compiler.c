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

	for (int i = 0; i <= program->tokensPointer; i++) {
		printf("%i \n", *(program->tokens + 1));
		getchar();
	}
}

void CompileFile(char * sourcePath)
{
	//Load source from file
	FILE *source = fopen(sourcePath, "r");
	fseek(source, 0, SEEK_END);
	long size = ftell(source);
	fseek(source, 0, SEEK_SET);
	char* buffer = calloc(sizeof(char),size + 1);
	fread(buffer, 1, size, source);
	fclose(source);
	buffer[size] = 0;

	Compile(buffer);
}

void CompileAssembly(char* source)
{



}

void CompileAssemblyFromFile(char* sourcePath)
{
	//Load source from file
	FILE * source;
	source = fopen(sourcePath, "rb");
	fseek(source, 0, SEEK_END);
	long size = ftell(source);
	fseek(source, 0, SEEK_SET);
	char* buffer = malloc(size + 1);
	fread(buffer, 1, size, source);

	CompileAssembly(buffer);
}
