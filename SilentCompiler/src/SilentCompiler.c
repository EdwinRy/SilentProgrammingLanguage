#include "SilentHelper.h"
#include "SilentTokenizer.h"
#include "SilentParser.h"
#include "SilentBytecode.h"
#include <stdio.h>

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
		break;

		case 2:
			outputCode = silentGenerateLibrary(program);
		break;
	}
	silentWriteOutput(output, outputCode);
}

//Testing
int main()
{
	char* source = "test.silent";
	char* out = "out";
	char compileOption = 1;
	silentCompile(source,out,compileOption);
	return 0;
}
