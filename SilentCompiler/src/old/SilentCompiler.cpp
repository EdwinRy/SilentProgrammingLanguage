#include "SilentCompiler.hpp"
#include "SilentTokenizer.hpp"
#include "SilentFiles.hpp"
namespace Silent
{
    Compiler::Compiler()
    {

    }

    std::string Compiler::Compile(std::string source)
    {
        std::string output;

        //Tokenize input source
        Tokenizer tokenizer;
        std::vector<Token> tokens = tokenizer.Tokenize(source);

        return output;
    }
}