#include <iostream>
#include <vector>
#include "SilentCompiler.hpp"
#include "SilentHelper.hpp"
#include "SilentTokenizer.hpp"
#include "SilentParser.hpp"
using namespace FileHelper;
using namespace SilentTokenizer;
using namespace SilentParser;
SilentCompiler::SilentCompiler()
{
	this->success = 0;
	this->outputAssembly = false;
	this->outputBytecode = false;
	this->inFilePath = "";
	this->outFilePath = "";
	this->source = "";
	this->bytecodeOutput = "";
	this->assemblyOutput = "";
}

void SilentCompiler::compile()
{
	if(!this->outputAssembly || !this->outputBytecode)
	{
		this->success = false;
		this->errorMessage = "No output method set";
	}

	std::vector<silentToken> tokens;
	if(inFilePath != "")
	{
		char* source = readAllText((char*)this->inFilePath.data());
		tokens = *silentTokenize(source);
	}
	else{
		tokens = *silentTokenize(this->source);
	}
	silentProgram program = *silentParseProgram(tokens);

	printf("Done!\n");

}

void SilentCompiler::setInFile(std::string path)
{
	this->inFilePath = path;
}

void SilentCompiler::setOutFile(std::string path)
{
	this->outFilePath = path;
}

void SilentCompiler::setInSource(std::string source)
{
	this->source = source;
}

void SilentCompiler::setOutputAssembly(bool flag)
{
	this->outputAssembly = flag;
}

void SilentCompiler::setOutputBytecode(bool flag)
{
	this->outputBytecode = flag;
}

bool SilentCompiler::getSuccessfull()
{
	return this->success;
}