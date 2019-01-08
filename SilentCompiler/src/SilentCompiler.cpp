#include "SilentCompiler.hpp"
#include "SilentTokenizer.hpp"
#include "SilentParser.hpp"
#include "SilentCodeGen.hpp"
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

        //std::vector<SilentToken>* tokens = tokenizer.Tokenize(this->source);
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
        char* output = codeGen.GetOutput(&codeLen);
        writeAllBytes("package.si", output, codeLen);

        //SilentCodeGenerator codeGen;
        //codeGen.Compile(&parser);

        //std::string intCode = SilentGenerateIntCode(parser.GetGlobalNamespace());

        //writeAllText("package.si", codeGen.GetOutput());
        //SilentParserInfo* parserOutput = SilentParse(tokenizer.GetTokens());
        //std::string intCode = SilentGenerateIntCode(parserOutput);
        //this->libOutput = intCode;
        //this->output = SilentCompileAST(*parserOutput);
        //this->output = SilentGenerateAssembly(intCode->code);

        //SilentCleaner cleaner;
        //cleaner.CleanupParser(&parser);
    }
    else
    {
        //SilentAssembler assembler = SilentAssembler();
        //assembler.Assemble(this->path);
    }
    std::cout << "\nDone!\n";
}

void SilentCompiler::SetSource(char* source) { this->source = source; }
void SilentCompiler::SetPath(char* path) { this->path = path; }

int main(int argc, char** argv)
{
    SilentCompiler compiler = SilentCompiler();
    
    if(strcmp(argv[1], "-c") == 0)
    {
        //SilentConsole console;
        //console.Start();
    }
    //else if(argc > 1)
    //{
    //    for(int i = 0; i < argc; i++)
    //    {
    //        if(strcmp(argv[1], "-o"))
    //    }
    //}
    else
    {
        compiler.SetPath(argv[1]);
        compiler.Compile(SilentCompileMode::Src);
    }
    return 0;
}