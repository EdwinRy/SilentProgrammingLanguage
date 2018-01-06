#include "SilentBytecode.h"
#include <stdio.h>
#include <stdlib.h>
vector* silentGenerateBytecode(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    return output;
}

vector* silentGenerateAssembly(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    return output;
}

vector* silentGenerateLibrary(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    return output;
}

void silentWriteOutput(char* outFile, vector* outStream)
{
    FILE* outputFile = fopen(outFile,"w");
    for(int i = 0; i < outStream->dataCount; i++)
    {
        fprintf(outputFile,"%s\n",outStream->strings[i]);
    }
    fclose(outputFile);
}