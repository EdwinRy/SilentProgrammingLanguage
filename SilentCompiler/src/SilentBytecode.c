#include "SilentBytecode.h"
#include <stdio.h>
#include <stdlib.h>
unsigned int getTypeSize(silentVariable* variable)
{
    unsigned int size = 0;

    return size;
}
vector* silentGenerateBytecode(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    return output;
}

vector* silentGenerateAssembly(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    //Current information about program
    unsigned int globalVariableCount = program->variables->dataCount;

    char* buffer[1024];

    //Write global variables
    for(int i = 0; i < program->variables->dataCount; i++)
    {
        unsigned int size = 
            getTypeSize((silentVariable*)program->variables[i].characters);
        if(size == 4)
        {

        }
        else if(size == 8)
        {

        }
        else
        {

        }
    }
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