#include <string>
#include <ctype.h>
#include "SilentVector.h"
#include <iostream>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

enum class SilentDataType
{
	INT8 = 0,
	UINT8,
    INT16,
	UINT16,
    INT32,
	UINT32,
	INT64,
    UINT64,
    FLOAT32,
    FLOAT64,
    POINTER,
    POINTER_LOCATION,
	UNDEFINED,
	UNDEFINED_END
};

enum class SilentBytecode
{
	Halt = 0,
	Goto,
	Sweep,
	Call,
	Return,
	LoadDll,
	LoadDllFunc,
	FreeDll,
	PushData,
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
    std::string label;
    uint64 index;
}SilentLabel;

char* assemble(char* inFile, char* outFile)
{
    std::vector<char> program = std::vector<char>();
    SilentVector* program = SilentCreateVector(100,1);
    SilentVector* labels = SilentCreateVector(100,sizeof(SilentLabel));
    SilentVector* aLabels = SilentCreateVector(100,sizeof(SilentLabel));

    FILE* fileStream;
    fileStream = fopen(inFile,"r");
    if(fileStream==NULL)
    {
        printf("File %s could not be opened\n", inFile);
        return 0;
    }

    char* line = NULL;
    uint64 currentLine = 1;
    size_t s = 0;
    uint64 size = 0;
    size = getline(&line, &s, fileStream);

    char buffer[10000];
    char* instructions[5];
    uint64 iIndex = 0; //instruction index

    char tempChar;
    char* tempPtr;

    while(size != -1)
    {
        //Handle next line
        if(line[0] == '\n')
        {
            currentLine += 1;
            size = getline(&line, &s, fileStream);
            continue;
        }

        //Parse line
        for(uint64 i = 0; i < size; i++)
        {
            if(isdigit(line[i]) || isalpha(line[i]))
            {
                uint64 charCount = 0;
                char* value;

                while(
                    isalpha(line[i+charCount]) || 
                    isdigit(line[i+charCount]) || 
                    line[i+charCount] == ':'
                )
                {
                    buffer[charCount] = line[i+charCount];
                    charCount++;
                }

                instructions[iIndex] = malloc(charCount+1);
                memcpy(instructions[iIndex],buffer,charCount);
                instructions[iIndex][charCount] = '\0';
                i += charCount;
                iIndex++;
            }

            if(line[i] == '\"')
            {
                i++; 
                uint64 count;
                buffer[0] = 's';
                for(count = 1; line[i] != '\"'; count++)
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
            SilentLabel* label = malloc(sizeof(SilentLabel));
            label->index = program->ptr;
            label->label = malloc(size-2);
            memcpy(label->label, instructions[0], size-2);
            label->label[size-2] = '\0';
            tempPtr = label->label;
            SilentPushBack(labels, label);
            printf("label %s on index %i\n", label->label, label->index);
            free(label);
        }

        if(strcmp(instructions[0],"halt") == 0)
        {
            tempChar = (char)Halt;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"goto") == 0)
        {
            tempChar = (char)Goto;
            SilentPushBack(program, &tempChar);
/*
            SilentLabel* label = malloc(sizeof(SilentLabel));
            label->index = program->ptr;
            label->label = malloc(size-2);
            memcpy(label->label, instructions[1], size-2);
            label->label[size-2] = '\0';
            SilentPushBack(aLabels,label);
            free(label);
*/
            uint64 temp = 0;
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"sweep") == 0)
        {
            tempChar = (char)Sweep;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"call") == 0)
        {
            tempChar = (char)Sweep;
            SilentPushBack(program, &tempChar);
            //Sub name
            uint64 temp = 0;
            SilentPushMultiple(program, 8, &temp);
            //Arg size
            temp = (uint64)atol(instructions[2]);
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"return") == 0)
        {
            tempChar = (char)Return;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"loaddll") == 0)
        {
        }

        if(strcmp(instructions[0],"loaddllfunc") == 0)
        {
        }

        if(strcmp(instructions[0],"freedll") == 0)
        {
        }

        if(strcmp(instructions[0],"push") == 0)
        {
            tempChar = (char)PushData;
            SilentPushBack(program, &tempChar);
            if(strcmp(instructions[1],"int8") == 0)
            {
                SilentPushBack(program, &dt[0]);
                char temp = (char)atoi(instructions[2]);
                SilentPushBack(program, &temp);
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                SilentPushBack(program, &dt[1]);
                unsigned char temp = (unsigned char)atoi(instructions[2]);
                SilentPushBack(program, &temp);
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                SilentPushBack(program, &dt[2]);
                short temp = (short)atoi(instructions[2]);
                SilentPushMultiple(program, 2, &temp);
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                SilentPushBack(program, &dt[3]);
                unsigned short temp = (unsigned short)atoi(instructions[2]);
                SilentPushMultiple(program, 2, &temp);
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                SilentPushBack(program, &dt[4]);
                int temp = (int)atoi(instructions[2]);
                SilentPushMultiple(program, 4, &temp);
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                SilentPushBack(program, &dt[5]);
                unsigned int temp = (unsigned int)atoi(instructions[2]);
                SilentPushMultiple(program, 4, &temp);
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                SilentPushBack(program, &dt[6]);
                long long temp = (long long)atol(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                SilentPushBack(program, &dt[7]);
                unsigned long long temp = (unsigned long long)atol(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                SilentPushBack(program, &dt[8]);
                float temp = (float)atof(instructions[2]);
                SilentPushMultiple(program, 4, &temp);
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                SilentPushBack(program, &dt[9]);
                double temp = (double)atof(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                SilentPushBack(program, &dt[10]);
                uint64 temp = (uint64)atol(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else
            {
                printf("Invalid type on line: %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"pop") == 0)
        {
            tempChar = (char)Pop;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"store") == 0)
        {
            tempChar = (char)Store;
            SilentPushBack(program, &tempChar);
            uint64 temp = (uint64)atol(instructions[1]);
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"load") == 0)
        {
            tempChar = (char)Load;
            SilentPushBack(program, &tempChar);
            if(strcmp(instructions[1],"int8") == 0)
            {
                SilentPushBack(program, &dt[0]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                SilentPushBack(program, &dt[1]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                SilentPushBack(program, &dt[2]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                SilentPushBack(program, &dt[3]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                SilentPushBack(program, &dt[4]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                SilentPushBack(program, &dt[5]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                SilentPushBack(program, &dt[6]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                SilentPushBack(program, &dt[7]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                SilentPushBack(program, &dt[8]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                SilentPushBack(program, &dt[9]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                SilentPushBack(program, &dt[10]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
        }

        if(strcmp(instructions[0],"storeglobal") == 0)
        {
            tempChar = (char)Store;
            SilentPushBack(program, &tempChar);
            uint64 temp = (uint64)atol(instructions[1]);
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"loadglobal") == 0)
        {
            tempChar = (char)LoadGlobal;
            SilentPushBack(program, &tempChar);
            if(strcmp(instructions[1],"int8") == 0)
            {
                SilentPushBack(program, &dt[0]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                SilentPushBack(program, &dt[1]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                SilentPushBack(program, &dt[2]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                SilentPushBack(program, &dt[3]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                SilentPushBack(program, &dt[4]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                SilentPushBack(program, &dt[5]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                SilentPushBack(program, &dt[6]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                SilentPushBack(program, &dt[7]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                SilentPushBack(program, &dt[8]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                SilentPushBack(program, &dt[9]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                SilentPushBack(program, &dt[10]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
        }

        if(strcmp(instructions[0],"alloc") == 0)
        {
            tempChar = (char)Alloc;
            SilentPushBack(program, &tempChar);
            uint64 temp = (uint64)atoi(instructions[1]);
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"loadptr") == 0)
        {
            tempChar = (char)LoadPtr;
            SilentPushBack(program, &tempChar);
            if(strcmp(instructions[1],"int8") == 0)
            {
                SilentPushBack(program, &dt[0]);
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                SilentPushBack(program, &dt[1]);
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                SilentPushBack(program, &dt[2]);
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                SilentPushBack(program, &dt[3]);
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                SilentPushBack(program, &dt[4]);
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                SilentPushBack(program, &dt[5]);
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                SilentPushBack(program, &dt[6]);
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                SilentPushBack(program, &dt[7]);
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                SilentPushBack(program, &dt[8]);
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                SilentPushBack(program, &dt[9]);
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                SilentPushBack(program, &dt[10]);
            }
            else if(strcmp(instructions[1],"str") == 0)
            {
                SilentPushBack(program, &dt[12]);
                uint64 temp = (uint64)atoi(instructions[2]);
                SilentPushMultiple(program, 8, &temp);
            }
            else
            {
                printf("Invalid type on line: %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"storeptr") == 0)
        {
            tempChar = (char)StorePtr;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"getptr") == 0)
        {
            tempChar = (char)GetPtr;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"freeptr") == 0)
        {
            tempChar = (char)FreePtr;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"free") == 0)
        {
            tempChar = (char)Free;
            SilentPushBack(program, &tempChar);
        }


        if(strcmp(instructions[0],"add") == 0)
        {
            tempChar = (char)Add;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"sub") == 0)
        {
            tempChar = (char)Sub;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"mul") == 0)
        {
            tempChar = (char)Mul;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"div") == 0)
        {
            tempChar = (char)Div;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"Convert") == 0)
        {
        }

        if(strcmp(instructions[0],"smallerthan") == 0)
        {
            tempChar = (char)SmallerThan;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"largerthan") == 0)
        {
            tempChar = (char)LargerThan;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"equal") == 0)
        {
            tempChar = (char)Equal;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"if") == 0)
        {
            tempChar = (char)If;
            SilentPushBack(program, &tempChar);
            uint64 temp = 0;
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"ifn") == 0)
        {
            tempChar = (char)IfNot;
            SilentPushBack(program, &tempChar);
            uint64 temp = 0;
            SilentPushMultiple(program, 8, &temp);
        }

        if(strcmp(instructions[0],"and") == 0)
        {
            tempChar = (char)And;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"or") == 0)
        {
            tempChar = (char)Or;
            SilentPushBack(program, &tempChar);
        }

        if(strcmp(instructions[0],"not") == 0)
        {
            tempChar = (char)Not;
            SilentPushBack(program, &tempChar);
        }

        currentLine += 1;
        size = getline(&line,&s,fileStream);
        for(int i = 0; i < iIndex; i++)
        {
            free(instructions[i]);
        }
        iIndex = 0;
    }

    //Link labels
    SilentLabel* label2;
    memcpy(label2, labels->data, sizeof(SilentLabel));
    printf("%i\n",labels->dataSize);
    printf("%i\n",labels->ptr);

    for(uint64 i = 0; i < aLabels->ptr; i++)
    {
        for(uint64 j = 0; j < labels->ptr; j++)
        {
        }
    }

    //Write to file
    FILE* out = fopen(outFile,"wb");
    fwrite(program->data, 1, program->ptr, out);
    fclose(out);
}

int main(int argc, const char** argv)
{
    char* in  = argv[1];
    char* out = argv[2];
    assemble(in, out);
    printf("\nDone!\n");
	return 0;
}