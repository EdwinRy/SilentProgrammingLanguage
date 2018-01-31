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

void silentWriteGlobals(silentProgram* program, vector* toWrite)
{
    //Buffer for the instruction
    char buffer[1024];
    //Write global variables
    for(int i = 0; i < program->variables->dataCount; i++)
    {
        //Make sure buffer is empty
        memset(buffer,0,1024);
        //Get variable size
        unsigned int varSize = 
            ((silentVariable*)vectorGet(program->variables,i))->value.size;
        if(varSize == 4)
        {
            sprintf(buffer,"alloc4 i%i",i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size+1] = '\0';
            vectorPushBack(toWrite,&value);
        }
        else if(varSize == 8)
        {
            sprintf(buffer,"alloc8 i%i",i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size+1] = '\0';
            vectorPushBack(toWrite,&value);
        }
        else
        {
            sprintf(buffer,"allocx i%i i%i",varSize,i);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size+1] = '\0';
            vectorPushBack(toWrite,&value);
        }
    }
}

void silentWriteExpression(silentFunction* function, vector* toWrite)
{

}

void silentWriteFunction(silentFunction* function, vector* toWrite)
{
    //Buffer for the instruction
    char buffer[1024];

    //Write function name
    sprintf(buffer,"%s:",function->name);
    int size = strlen(buffer);
    char* value = malloc(size+1);
    memcpy(value,buffer,size);
    value[size] = '\0';
    vectorPushBack(toWrite,&value);

    //Write parameters
    for(int j = 0; j < function->parameters->dataCount; j++)
    {
        //memset(buffer,0,1024);
        unsigned int varSize = 
            ((silentVariable*)vectorGet(function->parameters,j))->value.size;
        if(varSize == 4)
        {
            sprintf(buffer,"store4 i%i",j);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size] = '\0';
            vectorPushBack(toWrite,&value);
        }
        else if(varSize == 8)
        {
            sprintf(buffer,"store8 i%i",j);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size] = '\0';
            vectorPushBack(toWrite,&value);
        }
        else
        {
            sprintf(buffer,"storex i%i i%i",varSize,j);
            int size = strlen(buffer);
            char* value = malloc(size+1);
            memcpy(value,buffer,size);
            value[size] = '\0';
            vectorPushBack(toWrite,&value);
        }
    }

    for(int j = 0; j < function->expressions->dataCount; j++)
    {
        silentWriteExpression(function, toWrite);
    }
}

//Generate assembly-like code for the VM
vector* silentGenerateAssembly(silentProgram* program)
{
    vector* output = createVector(sizeof(char*));
    //Current information about program
    unsigned int globalVariableCount = program->variables->dataCount;
    //Buffer for the instruction
    char buffer[1024];
    
    //Write globals
    silentWriteGlobals(program, output);

    //Write functions
    for(int i = 0; i < program->functions->dataCount; i++)
    {
        silentWriteFunction(vectorGet(program->functions, i), output);
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