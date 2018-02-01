#include "SilentCompiler.hpp"
SilentCompiler::SilentCompiler()
{
	this->success = 0;
	this->outputAssembly = 0;
	this->outputBytecode = 0;
	this->usingInFile = 0;
	this->usingOutFile = 0;
	this->inFilePath = "";
	this->outFilePath = "";
	this->source = "";
	this->bytecodeOutput = "";
	this->assemblyOutput = "";
}

void SilentCompiler::compile()
{

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

std::string SilentCompiler::getOutput()
{
	return this->output;
}

bool SilentCompiler::getSuccessfull()
{
	return this->success;
}