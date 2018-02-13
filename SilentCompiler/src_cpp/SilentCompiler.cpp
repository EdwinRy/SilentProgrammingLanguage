#include <iostream>
#include <vector>
#include "SilentCompiler.hpp"
#include "SilentHelper.hpp"
#include "SilentWriter.hpp"
#include "SilentTokenizer.hpp"
using namespace FileHelper;
using namespace SilentTokenizer;
using namespace SilentParser;
using namespace SilentCodeGenerator;
SilentCompiler::SilentCompiler()
{
	this->success = 0;
	this->outputAssembly = false;
	this->outputBytecode = false;
	this->inFilePath = "";
	this->outFilePath = "";
	this->source = "";
	this->bytecodeOutput = "";
}

void SilentCompiler::compile()
{
	if(!this->outputAssembly && !this->outputBytecode)
	{
		this->success = false;
		this->errorMessage = "No output method set";
	}

	if(this->outputAssembly && this->outputBytecode)
	{
		this->success = false;
		this->errorMessage = "Only one output allowed at a time";
	}

	std::vector<silentToken> tokens;
	
	if(this->outputAssembly)
	{
		if(this->inFilePath != "")
		{
			char* source = readAllText((char*)this->inFilePath.data());
			tokens = *silentTokenize(source);
		}
		else{
			tokens = *silentTokenize(this->source);
		}
		silentProgram program = *silentParseProgram(tokens);

		if(this->outFilePath != "")
		{

		}
		else
		{
			this->assemblyOutput = compileAssembly(program);
			printf("assembly output:\n");
			for(unsigned int i = 0; i < this->assemblyOutput.size();i++)
			{
				printf("%s\n",this->assemblyOutput[i].data());
			}
		}
	}
	else if(this->outputBytecode)
	{
		
	}

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