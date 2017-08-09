#include "Compiler.h"

void Compile(char * source)
{
	Program* program = malloc(sizeof(Program));
	program->source = source;

	Tokenize(program->source, program->tokens);
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
