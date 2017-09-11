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


void CompileAssembly(assembly* asm)
{

	printf(asm->outputFile);

}