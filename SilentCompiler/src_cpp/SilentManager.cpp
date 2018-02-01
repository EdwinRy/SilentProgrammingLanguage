#include <iostream>
#include <vector>
#include "SilentReader.hpp"
#include "SilentTokenizer.hpp"

using namespace SilentTokenizer;
char* silentCompile(char* sourcePath, char* outputPath, char compileOption)
{
	char* rawSource = FileReader::readAllText(sourcePath);
	std::vector<silentToken> *tokens = silentTokenize(rawSource);
	//silentProgram* 	program 	= silentParseProgram(tokens,tokenCount);
	//vector* 		outputCode;

    /*
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
    */
	printf("Done!\n");
}

int main()
{
    char* 	source 			= (char*)"test.silent";
	char* 	out 			= (char*)"output.test";
	char 	compileOption 	= 1;
	silentCompile(source,out,compileOption);
    return 0;
}