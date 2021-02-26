#include "SilentC.h"
#include "parser.h"

SilentCompileInfo SilentCompile(char* source)
{
    SilentCompileInfo info = {0};
    info.tokenizer = Tokenize(source);
    PrintTokens(info.tokenizer.tokens, info.tokenizer.count);
    info.symTable = SymbolTableLoad(info.tokenizer.symLink);
    SilentParse(&info);
    PrintParserAst(info.parser->root);
    return info;
}