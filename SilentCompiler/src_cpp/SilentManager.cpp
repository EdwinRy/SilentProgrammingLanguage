#include <iostream>
#include "SilentCompiler.hpp"

int main()
{
    char* 	source 			= (char*)"test.silent";
	char* 	out 			= (char*)"output.test";
	SilentCompiler compiler;
	compiler.setInFile(source);
	compiler.setOutFile(out);
	compiler.setOutputAssembly(true);
	compiler.compile();

    return 0;
}