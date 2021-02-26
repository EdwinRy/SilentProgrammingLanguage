#include "silent_compiler.h"
#include "silent_lexer.h"
#include "stdlib.h"

SilentCompiler CompilerInit(char* src)
{
    SilentCompiler compiler = {0};
    Tokenizer tokenizer = TokenizerInit(src);
    compiler.parser = ParserInit(tokenizer);
    return compiler;
}

char* CompileBytes(SilentCompiler* compiler)
{
    ParserTest(compiler->parser);
    return NULL;
}