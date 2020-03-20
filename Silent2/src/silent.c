#include "silent.h"
#include "silent_compiler.h"

SilentProgram SilentCompileSource(char* source)
{
    SilentProgram program;
    program.bytes = CompileBytes(source);
    return program;
}