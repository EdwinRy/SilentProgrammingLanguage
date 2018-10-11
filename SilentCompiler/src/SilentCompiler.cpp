#include "SilentCompiler.hpp"
#include "SilentTokenizer.hpp"
#include "SilentParser.hpp"
#include "SilentCleanup.hpp"
#include "Assembler/SilentAssembler.hpp"
#include "SilentCodeGen.hpp"
#include "SilentIntCode.hpp"
#include "SilentConsole.hpp"
#include "SilentFiles.hpp"
#include <iostream>
#include <vector>
using namespace Silent;

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
        std::string intCode = SilentGenerateIntCode(parserOutput);
        writeAllText("package.spck", intCode.data());
        this->libOutput = intCode;
        this->output = SilentCompileAST(*parserOutput);
        //this->output = SilentGenerateAssembly(intCode->code);
        SilentCleanupParserInfo(parserOutput);
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