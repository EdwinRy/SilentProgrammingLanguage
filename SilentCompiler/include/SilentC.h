#pragma once
#include "common.h"
#include "lexer.h"
#include "parser.h"

typedef struct ParseInfo ParseInfo;
typedef struct SilentProgram SilentProgram;

typedef struct SilentCompileInfo{
    SymbolTable* symTable;
    TokenizerInfo tokenizer;
    ParseInfo* parser;
    SilentProgram* program;
}SilentCompileInfo;

SilentCompileInfo SilentCompile(char* source);

typedef struct ParseNode ParseNode;
void PrintParserAst(ParseNode* node);
void PrintTokens(Token* tokens, uint64 count);