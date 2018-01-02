#include "SilentHelper.h"
#include "SilentTokenizer.h"
#include "SilentParser.h"
#include <stdio.h>

char* silentCompile(char* path, char* output)
{
	char* rawSource = readAllText(path);//no AST
	int tokenCount = 0;
	silentToken* tokens = silentTokenize(rawSource,&tokenCount);
	silentParseProgram(tokens,tokenCount);
}

//Testing
int main()
{
	char* source = "test.silent";
	char* out = "out";
	silentCompile(source,out);
	return 0;
}
