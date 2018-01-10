#include "SilentHelper.h"
#include "SilentTokenizer.h"
#include "SilentParser.h"
#include "SilentBytecode.h"
#include <stdio.h>

//Compile silent program
char* silentCompile(char* path, char* output, char compileOption)
{
	char* rawSource = readAllText(path);//no AST
	int tokenCount = 0;
	silentToken* tokens = silentTokenize(rawSource,&tokenCount);
	silentProgram* program = silentParseProgram(tokens,tokenCount);
	vector* outputCode;
	switch(compileOption)
	{
		case 0:
			outputCode = silentGenerateBytecode(program);
		break;

		case 1:
			outputCode = silentGenerateAssembly(program);
			silentWriteAssembly(output, outputCode);
		break;

		case 2:
			outputCode = silentGenerateLibrary(program);
		break;

		case 3:
			outputCode = silentGenerateBytecode(program);
			return outputCode->characters;
		break;
	}
	printf("Done!\n");
}

//Handle Compilation input/outputs
int main()
{
	char* source = "test.silent";
	char* out = "output.test";
	char compileOption = 1;
	silentCompile(source,out,compileOption);
	return 0;
}
