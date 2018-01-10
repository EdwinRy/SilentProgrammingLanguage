#include "SilentBytecode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Keep reference to global structures
vector* structs;

//Generate code to be executed by the VM
vector* silentGenerateBytecode(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    return output;
}

//Recursive expression generation
vector* silentTransformExpression(silentExpression* expression)
{
    vector* instructions = createVector(sizeof(char*));
    //Expression code generating
    return instructions;
}

//Generate assembly-like code for the VM
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
        unsigned int varSize = 
            ((silentVariable*)vectorGet(program->variables,i))->value.size;
        if(varSize == 4)
        {
            sprintf(buffer,"alloc4 i%i",i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size+1] = '\0';
            vectorPushBack(output,&value);
        }
        else if(varSize == 8)
        {
            sprintf(buffer,"alloc8 i%i",i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size+1] = '\0';
            vectorPushBack(output,&value);
        }
        else
        {
            sprintf(buffer,"allocx i%i i%i",varSize,i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size+1] = '\0';
            vectorPushBack(output,&value);
        }
    }

    //Write functions
    for(int i = 0; i < program->functions->dataCount;i++)
    {
        silentFunction* currentFunc = 
            (silentFunction*)(vectorGet(program->functions,i));
        //Write function name
        sprintf(buffer,"%s:",currentFunc->name);
        int size = strlen(buffer);
        char* value = malloc(size+1);
        memcpy(value,buffer,size);
        value[size+1] = '\0';
        vectorPushBack(output,&value);
        for(int j = 0; i < currentFunc->expressions->dataCount; i++)
        {
            silentExpression* currentExpression = 
                (silentExpression*)(vectorGet(currentFunc->expressions,i));
        }
    }

    return output;
}

//Generate package used to link with other files
vector* silentGenerateLibrary(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    return output;
}

//Write bytecode to file
void silentWriteBytecode(char* outFile, vector* outStream)
{

}

//Write assembly to file line by line
void silentWriteAssembly(char* outFile, vector* outStream)
{
    printf("Output:\n");
    FILE* outputFile = fopen(outFile,"w");
    for(int i = 0; i < outStream->dataCount; i++)
    {
        printf("%s\n",outStream->strings[i]);
        fprintf(outputFile,"%s\n",(char*)outStream->strings[i]);
    }
    fprintf(outputFile, "halt");
    fclose(outputFile);
}

//Write library file
void silentWriteLibrary(char* outFile, vector* outStream)
{

}