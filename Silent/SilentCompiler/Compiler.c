#include "Compiler.h"

void Compile(char * source)
{
	Program* program = malloc(sizeof(Program));
	program->source = source;
	program->tokensPointer = 0;
	program->bytecodePointer = 0;
	program->valuePointer = 0;
	program->sourceLength = strlen(source);

	Tokenize(program);
}

void CompileFile(char * sourcePath)
{
	//Load source from file
	FILE * source;
	source = fopen(sourcePath, "rb");
	fseek(source, 0, SEEK_END);
	long size = ftell(source);
	fseek(source, 0, SEEK_SET);
	char* buffer = malloc(size + 1);
	fread(buffer, 1, size, source);

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
