#include <stdio.h>
#include <stdlib.h>
#include <ctype>
#include <string.h>
#include "SilentVector.h"

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

typedef enum SilentBytecode
{
	Halt = 0,
	Goto,
	Sweep,
	Call,
	Return,
	LoadDll,
	LoadDllFunc,
	FreeDll,
	Push,
	Pop,
	Store,
	Load,
	StoreGlobal,
	LoadGlobal,
	Alloc,
	LoadPtr,
	StorePtr,
	FreePtr,
	Free,
	GetPtr,
	Add,
	Sub,	
	Mul,	
	Div,
	Convert,
	SmallerThan,
	LargerThan,
	Equal,
	If,
	IfNot,
	And,
	Or,
	Not
}SilentBytecode;

typedef struct SilentLabel
{
    char* label;
    uint64 index;
}SilentLabel;

char* assemble(char* inFile)
{
    SilentVector* program = SilentCreateVector(100,1);
    SilentVector* labels = SilentCreateVector(100,sizeof(SilentLabel));

    FILE* f;
    f = open(inFile,"r");
    if(f==NULL)
    {
        printf("File %s could not be opened\n", inFile);
        return 0;
    }

    char* line;
    uint64 currentLine = 0;
    size_t s = 0;
    uint64 size = (uint64)getLine(&line, &s, f);

    char buffer[10000];
    char* instructions[5];
    uint64 iIndex = 0; //instruction index

    char tempChar;

    while(size != -1)
    {
        //Handle next line
        if(line[0] == '\n')
        {
            currentLine += 1;
            size = getLine(&line, &s, f);
            continue;
        }
        
        //Parse line
        for(uint64 i = 0; i < size; i++)
        {
            if(isalpha(line[i]))
            {
                uint64 charCount = 0;
                char* value;

                while(isalpha(line[i+count]) || isdigit(line[i+count]) || line[i+count] == ':')
                {
                    buffer[count] = line[i+count];
                    count++;
                }

                instructions[iIndex] = malloc(count+1);
                memcpy(instructions[iIndex],buffer,count);
                instructions[iIndex][count] = '\0';
                i += count;
                iIndex++;
            }

            if(line[i] == "\"")
            {
                i++; 
                uint64 count;
                buffer[0] = 's';
                for(count = 1; line[i] != "\""; count++)
                {
                    if(line[i] == '\\')
                    {
                        i += 1;
                        if(line[i] == 'n')
                        {
                            buffer[count] = '\n';
                        }
                        else if(line[i] == 't')
                        {
                            buffer[count] = '\t';
                        }
                        else if(line[i] == '\\')
                        {
                            buffer[count] = '\\';
                        }
                        else if(line[i] == 'v')
                        {
                            buffer[count] = '\v';
                        }
                        else if(line[i] == 'r')
                        {
                            buffer[count] = '\r';
                        }
                        else if(line[i] == 'f')
                        {
                            buffer[count] = '\f';
                        }
                        else if(line[i] == '0')
                        {
                            buffer[count] = '\0';
                        }
                        else{
                            buffer[count] = line[i];
                        }
                    }
                    else{
                        buffer[count] = line[i];
                    }
                    i++;
                }
                instructions[iIndex] = malloc(count);
                memcpy(instructions[iIndex], buffer, count);
                i += count;
                iIndex += 1;
            }
        }

        if(instructions[0][size-2] == ':')
        {
            SilentLabel label = malloc(sizeof(SilentLabel));
            label->index = program->ptr;
            label->label = malloc(size-2);
            memcpy(label->label, instructions[0], size-2);
            label->label[size-2] = '\0';
            SilentPushBack(labels, label);
            free(label);
        }

        if(strcmp(instructions[0],"halt "))
        {
            tempChar = (Halt);
            SilentPushBack(program, &tempChar);
        }

    }
}