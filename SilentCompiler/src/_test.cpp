#include "SilentCompiler.h"
#include <iostream>


int main()
{	
	std::string source = "test.silent";
	std::string output = "out";
	silentCompile(source, output);
	return 0;
}
