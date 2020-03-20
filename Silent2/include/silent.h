#pragma once

typedef struct SilentProgram
{
    char* bytes;
}SilentProgram;

SilentProgram SilentCompileSource(char* source);