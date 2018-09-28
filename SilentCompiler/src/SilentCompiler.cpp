#include "SilentCompiler.hpp"
#include "SilentTokenizer.hpp"
#include "SilentParser.hpp"
#include "SilentCleanup.hpp"
#include "Assembler/SilentAssembler.hpp"
#include "SilentCodeGen.hpp"
#include "SilentIntCode.hpp"
#include "SilentConsole.hpp"
#include <iostream>
#include <string.h>
#include <vector>
using namespace Silent;

char* readAllText(char* path)
{
    char* text;
    FILE *f;

    if((f = fopen(path,"r"))==NULL)
    {
        printf("File %s doesn't exist!\n", path);
        exit(-1);
    }
    fseek(f,0,SEEK_END);
    text = (char*)malloc(ftell(f) + 1);
    fseek(f,0,SEEK_SET);
    long count = 0;
    char c;
    while((c = fgetc(f))!=EOF) { text[count++] = (char)c; }
    text[count] = '\0';
    fclose(f);
    return text;
}

SilentCompiler::SilentCompiler() { this->source = ""; }

void SilentCompiler::Compile(SilentCompileMode mode)
{
    if(mode == SilentCompileMode::Src)
    {
        if(this->source == "")
        {
            char* source = readAllText(this->path);
            this->source.assign(source,strlen(source)+1);
        }
        std::vector<SilentToken>* tokens = SilentTokenize(this->source);
        SilentParserInfo* parserOutput = SilentParse(*tokens);
        SilentIntCode* intCode = SilentTransform(parserOutput);
        this->libOutput = intCode->code;
        this->output = SilentCompileIntCode(intCode->code);
        //this->output = SilentGenerateAssembly(intCode->code);
        SilentCleanup(parserOutput);
    }
    else
    {
        SilentAssembler assembler = SilentAssembler();
        assembler.Assemble(this->path);
    }
    std::cout << "\nDone!\n";
}

void SilentCompiler::SetSource(char* source) { this->source = source; }
void SilentCompiler::SetPath(char* path) { this->path = path; }

int main(int argc, char** argv)
{
    SilentCompiler compiler = SilentCompiler();
    if(strcmp(argv[1], "-asm") == 0)
    {
        compiler.SetPath(argv[2]);
        compiler.Compile(SilentCompileMode::Asm);
    }
    if(strcmp(argv[1], "-c") == 0)
    {
        SilentConsole console;
        console.Start();
    }
    else
    {
        compiler.SetPath(argv[1]);
        compiler.Compile(SilentCompileMode::Src);
    }
    return 0;
}