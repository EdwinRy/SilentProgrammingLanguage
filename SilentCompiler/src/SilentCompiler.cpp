#include "SilentCompiler.hpp"
#include "SilentTokenizer.hpp"
#include "SilentParser.hpp"
#include "SilentCleanup.hpp"
#include "Assembler/SilentAssembler.hpp"
#include <iostream>
#include <string.h>
#include <vector>
using namespace Silent;

char* readAllText(char* path)
{
    //Text buffer to be returned
    char* text;
    //File to be read from
    FILE *f;
    //If opening of the file fails
    if((f = fopen(path,"r"))==NULL)
    {
        //output about file not existing
        printf("File %s doesn't exist!\n",path);
        //Close the program
        exit(-1);
    }
    //Seek the end of the file
    fseek(f,0,SEEK_END);
    //Allocate enough space for the entire file
    text = (char*)malloc(ftell(f));
    //Rewind the file back to beginning
    fseek(f,0,SEEK_SET);
    //Declare counter for the character array
    long count = 0;
    //Initialise buffer for single character
    char c;
    //Iterate for as long as the character is not
    //the end of the file
    while((c = fgetc(f))!=EOF)
    {
        //Add the character
        text[count++] = (char)c;
    }
    text[count] = '\0';
    //Close the file
    fclose(f);
    //Return the source
    return text;
}



SilentCompiler::SilentCompiler()
{
    this->source = "";
}

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
        SilentScope* nodes = SilentParse(*tokens);
        //SilentFreeNodes(nodes);
    }
    else
    {
        SilentAssembler assembler = SilentAssembler();
        assembler.Assemble(this->path);
    }
    std::cout << "\nDone!\n";
}

void SilentCompiler::SetSource(char* source)
{
    this->source = source;
}

void SilentCompiler::SetPath(char* path)
{
    this->path = path;
}

int main(int argc, char** argv)
{
    SilentCompiler compiler = SilentCompiler();
    if(strcmp(argv[1], "-asm") == 0)
    {
        compiler.SetPath(argv[2]);
        compiler.Compile(SilentCompileMode::Asm);
    }
    else
    {
        compiler.SetPath(argv[1]);
        compiler.Compile(SilentCompileMode::Src);
    }
    return 0;
}