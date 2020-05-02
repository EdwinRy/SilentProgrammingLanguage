#include "silent_compiler.h"
#include "silent_lexer.h"
#include "stdlib.h"

SilentCompiler CompilerInit(char* src)
{
    SilentCompiler compiler;
    Tokenizer tokenizer = TokenizerInit(src);
    compiler.parser = ParserInit(tokenizer);
}

char* CompileBytes(SilentCompiler* compiler)
{
    ParserTest(compiler->parser);
    return NULL;
}