#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "silent.h"
#include "silent_compiler.h"
#include "silent_files.h"

int main(int argc, char *argv[]) {

    char* src = readAllText("test.silent");
    SilentCompiler compiler = CompilerInit(src);
    char* out = CompileBytes(&compiler);

    return 0;
}