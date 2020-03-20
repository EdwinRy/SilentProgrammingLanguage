#include "compiler.hpp"

namespace Silent
{
    char* Compiler::GetBytes(char* source)
    {
        char* out;
        ParserStruct::Node parserAst = parser.GenerateAst(source);
        
        return out;
    }
}