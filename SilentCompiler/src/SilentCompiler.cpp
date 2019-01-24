#include "SilentCompiler.hpp"
#include "SilentTokenizer.hpp"
#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
#include "SilentFiles.hpp"
#include <iostream>
#include <vector>
using namespace Silent;

SilentCompiler::SilentCompiler() { this->source = ""; }

void SilentCompiler::Compile(char* inFile, char* outPath)
{
    char* fileContent = readAllText(inFile);
    std::string tempSource;
    tempSource.assign(fileContent, strlen(fileContent)+1);
    this->CompileSource(tempSource, outPath);
}

void SilentCompiler::CompileSource(std::string source, char* outFile)
{
    this->source = source;
    this->outPath = outFile;

    Tokenizer tokenizer;
    if(!tokenizer.Tokenize(this->source))
    {
        std::cout << "Could not tokenize source\n";
    }

    Parser parser;
    if(!parser.Parse(tokenizer.GetTokens()))
    {
        std::cout << "Parsing unsuccessful\n";
    }

    CodeGenerator codeGen;
    codeGen.GenerateBytecode(parser);
    uint64 codeLen;
    char* binOutput = codeGen.GetOutput(&codeLen);

    if(outFile != NULL)
    {
        writeAllBytes(outFile, binOutput, codeLen);
    }
    this->output.assign(binOutput, codeLen);
    std::cout << "\nDone!\n";
}

int main(int argc, char** argv)
{
    SilentCompiler compiler = SilentCompiler();
    
    char* outPath = NULL;
    char* inFile = NULL;

    for(uint64 i = 1; i < (uint64)argc; i++)
    {
        if(strcmp(argv[i], "-o") == 0) outPath = argv[++i];
        else inFile = argv[i];
    }
    
    if(inFile != NULL) compiler.Compile(inFile, outPath);

    return 0;
}