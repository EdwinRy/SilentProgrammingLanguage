#include "SilentBytecode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
vector* structs;
unsigned int calculateStructureSize(silentStruct* structure)
{

}
unsigned int getTypeSize(silentVariable* variable)
{
    unsigned int size = 0;

    switch(variable->value.type)
    {
        case silentInteger:
        case silentFloat:
            return 4;
        break;
        case silentLong:
        case silentDouble:
            return 8;
        break;
        case silentStructType:
            //return calculateStructureSize()
        break;
    }

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

    char buffer[1024];

    //Write global variables
    for(int i = 0; i < program->variables->dataCount; i++)
    {
        memset(buffer,0,1024);
        unsigned int size = 
            getTypeSize((silentVariable*)vectorGet(program->variables,i));
        if(size == 4)
        {
            sprintf(buffer,"alloc4 i%i",i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size] = '\0';
            vectorPushBack(output,value);
            free(value);
        }
        else if(size == 8)
        {
            sprintf(buffer,"alloc8 i%i",i);
        }
        else
        {
            sprintf(buffer,"allocx i%i i%i",size,i);
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
        printf("here\n");
        fprintf(outputFile,"%s\n",outStream->strings[i]);
    }
    fclose(outputFile);
}