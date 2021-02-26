#include "silent_parser.h"
#pragma once

typedef struct SilentCompiler {
    Parser parser;
}SilentCompiler;

SilentCompiler CompilerInit(char* src);
char* CompileBytes(SilentCompiler* compiler);