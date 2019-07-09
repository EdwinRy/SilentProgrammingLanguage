#include "SCompiler.hpp"
#include "SFiles.hpp"
#include "SParser.hpp"
#include "STokenizer.hpp"
#include "SCodeGen.hpp"
#include <iostream>
#include <vector>
using namespace Silent;

SilentCompiler::SilentCompiler() { this->source = ""; }

// Compile an array of files and output an executable
void SilentCompiler::Compile(std::vector<char*> inFiles, char* outPath)
{
    for(char* file : inFiles)
    {
        char* fileContent = readAllText(file);
        std::string tempSource;
        tempSource.assign(fileContent, strlen(fileContent)+1);
        this->CompileSource(tempSource, outPath);
    }
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
    Program* ast = parser.Parse(tokenizer.GetTokens());

    CodeGenerator codeGen;
    codeGen.GenBytecode(ast);
    // if(!parser.Parse(tokenizer.GetTokens()))
    // {
    //     std::cout << "Parsing unsuccessful\n";
    // }

    // CodeGenerator codeGen;
    // codeGen.GenerateBytecode(parser);
    // uint64 codeLen;
    // char* binOutput = codeGen.GetOutput(&codeLen);

    // if(outFile != NULL)
    // {
    //     writeAllBytes(outFile, binOutput, codeLen);
    // }
    // this->output.assign(binOutput, codeLen);
    std::cout << "\nDone!\n";
}

int main(int argc, char** argv)
{
    SilentCompiler compiler = SilentCompiler();
    
    char* outPath = NULL;
    // char* inFile = NULL;

    std::vector<char*> inFiles;

    for(unsigned long long i = 1; i < (unsigned long long)argc; i++)
    {
        if(strcmp(argv[i], "-o") == 0) outPath = argv[++i];
        // else inFile = argv[i];
        else inFiles.push_back(argv[i]);
    }
    
    if(inFiles.size() > 0) compiler.Compile(inFiles, outPath);

    return 0;
}