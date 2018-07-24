#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 0

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;

typedef enum SilentDataType
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
}dataType;

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
    char* label;
    uint64 index;
}SilentLabel;

char* assemble(char* inFile, char* outFile)
{
    char* program = malloc(10000);
    uint64 pc = 0;
    SilentLabel* labels = malloc(10000);
    uint64 labelIndex = 0;
    SilentLabel* aLabels = malloc(10000);
    uint64 aLabelIndex = 0;

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

    char buffer[1000];
    char** instructions = malloc(sizeof(char*)*5);
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
        #if DEBUG
            printf("l %s",line);
        #endif
        //Parse line
        for(uint64 i = 0; i < size; i++)
        {
            if(isdigit(line[i]) || isalpha(line[i]))
            {
                uint64 charCount = 0;
                buffer[0] = line[i+charCount];
                charCount+=1;
                char* value;

                while(
                    (isalpha(line[i+charCount]) || 
                    isdigit(line[i+charCount]) || 
                    line[i+charCount] == ':') && charCount < size
                )
                {
                    buffer[charCount] = line[i+charCount];
                    charCount++;
                }

                instructions[iIndex] = malloc(charCount+5);
                memcpy(instructions[iIndex], buffer, charCount);
                instructions[iIndex][charCount] = '\0';
                i += charCount;
                iIndex++;
            }


            else if(line[i] == '\"')
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
            SilentLabel label;
            label.index = pc;
            label.label = malloc(size-2);
            memcpy(label.label, instructions[0], size-2);
            label.label[size-2] = '\0';
            labels[labelIndex] = label;
            #if DEBUG
                printf("label %s on index %i\n", label.label, label.index);
            #endif
            labelIndex+=1;
        }

        if(strcmp(instructions[0],"halt") == 0)
        {
            program[pc++] = (char)Halt;
        }

        //printf("l %s\n",instructions[0]);

        if(strcmp(instructions[0],"goto") == 0)
        {
            program[pc++] = (char)Goto;
            aLabels[aLabelIndex].index = pc;
            aLabels[aLabelIndex].label = malloc(size-2);
            memcpy(aLabels[aLabelIndex].label, instructions[1], size-2);
            aLabels[aLabelIndex].label[size-2] = '\0';
            #if DEBUG
                printf("awaiting label %s at index %i\n", aLabels[aLabelIndex].label, aLabels[aLabelIndex].index);
            #endif
            aLabelIndex++;
            uint64 temp = 0;
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"sweep") == 0)
        {
            program[pc++] = (char)Sweep;
        }

        if(strcmp(instructions[0],"call") == 0)
        {
            program[pc++] = (char)Call;
            //label name
            uint64 temp = 0;
            memcpy(program + pc, &temp, 8);
            aLabels[aLabelIndex].index = pc;
            aLabels[aLabelIndex].label = malloc(size-2);
            memcpy(aLabels[aLabelIndex].label, instructions[1], size-2);
            aLabels[aLabelIndex].label[size-2] = '\0';
            #if DEBUG
                printf("awaiting label %s at index %i\n", aLabels[aLabelIndex].label, aLabels[aLabelIndex].index);
            #endif            
            aLabelIndex++;
            pc+=8;
            //Arg size
            temp = (uint64)atol(instructions[2]);
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"return") == 0)
        {
            program[pc++] = (char)Return;
            uint64 temp = (uint64)atol(instructions[1]);
            memcpy(program + pc, &temp, 8);
            pc+=8;
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
            program[pc++] = (char)PushData;
            if(strcmp(instructions[1],"int8") == 0)
            {
                program[pc++] = INT8;
                char temp = (char)atoi(instructions[2]);
                memcpy(program + pc, &temp, 1);
                pc+=1;
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                program[pc++] = UINT8;
                unsigned char temp = (unsigned char)atoi(instructions[2]);
                memcpy(program + pc, &temp, 1);
                pc+=1;
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                program[pc++] = INT16;
                short temp = (short)atoi(instructions[2]);
                memcpy(program + pc, &temp, 2);
                pc+=2;
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                program[pc++] = UINT16;
                unsigned short temp = (unsigned short)atoi(instructions[2]);
                memcpy(program + pc, &temp, 2);
                pc+=2;
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                program[pc++] = INT32;
                int temp = (int)atoi(instructions[2]);
                memcpy(program + pc, &temp, 4);
                pc+=4;
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                program[pc++] = UINT32;
                unsigned int temp = (unsigned int)atoi(instructions[2]);
                memcpy(program + pc, &temp, 4);
                pc+=4;
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                program[pc++] = INT64;
                long long temp = (long long)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                program[pc++] = UINT64;
                unsigned long long temp = (unsigned long long)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                program[pc++] = FLOAT32;
                float temp = (float)atof(instructions[2]);
                memcpy(program + pc, &temp, 4);
                pc+=4;
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                program[pc++] = FLOAT64;
                double temp = (double)atof(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                program[pc++] = POINTER;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else
            {
                printf("Invalid type on line: %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"pop") == 0)
        {
            program[pc++] = (char)Pop;
        }

        if(strcmp(instructions[0],"store") == 0)
        {
            program[pc++] = (char)Store;
            uint64 temp = (uint64)atol(instructions[1]);
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"load") == 0)
        {
            program[pc++] = (char)Load;
            if(strcmp(instructions[1],"int8") == 0)
            {
                program[pc++] = INT8;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                program[pc++] = UINT8;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                program[pc++] = INT16;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                program[pc++] = UINT16;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                program[pc++] = INT32;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                program[pc++] = UINT32;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                program[pc++] = INT64;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                program[pc++] = UINT64;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                program[pc++] = FLOAT32;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                program[pc++] = FLOAT64;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                program[pc++] = POINTER;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else
            {
                printf("Invalid type on line: %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"storeglobal") == 0)
        {
            program[pc++] = (char)StoreGlobal;
            uint64 temp = (uint64)atol(instructions[1]);
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"loadglobal") == 0)
        {
            program[pc++] = (char)LoadGlobal;
             if(strcmp(instructions[1],"int8") == 0)
            {
                program[pc++] = INT8;
                char temp = (char)atoi(instructions[2]);
                memcpy(program + pc, &temp, 1);
                pc+=1;
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                program[pc++] = UINT8;
                unsigned char temp = (unsigned char)atoi(instructions[2]);
                memcpy(program + pc, &temp, 1);
                pc+=1;
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                program[pc++] = INT16;
                short temp = (short)atoi(instructions[2]);
                memcpy(program + pc, &temp, 2);
                pc+=2;
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                program[pc++] = UINT16;
                unsigned short temp = (unsigned short)atoi(instructions[2]);
                memcpy(program + pc, &temp, 2);
                pc+=2;
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                program[pc++] = INT32;
                int temp = (int)atoi(instructions[2]);
                memcpy(program + pc, &temp, 4);
                pc+=4;
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                program[pc++] = UINT32;
                unsigned int temp = (unsigned int)atoi(instructions[2]);
                memcpy(program + pc, &temp, 4);
                pc+=4;
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                program[pc++] = INT64;
                long long temp = (long long)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                program[pc++] = UINT64;
                unsigned long long temp = (unsigned long long)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                program[pc++] = FLOAT32;
                float temp = (float)atof(instructions[2]);
                memcpy(program + pc, &temp, 4);
                pc+=4;
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                program[pc++] = FLOAT64;
                double temp = (double)atof(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                program[pc++] = POINTER;
                uint64 temp = (uint64)atol(instructions[2]);
                memcpy(program + pc, &temp, 8);
                pc+=8;
            }
            else
            {
                printf("Invalid type on line: %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"alloc") == 0)
        {
            program[pc++] = (char)Alloc;
            uint64 temp = (uint64)atoi(instructions[1]);
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"loadptr") == 0)
        {
            program[pc++] = (char)LoadPtr;
            if(strcmp(instructions[1],"int8") == 0)
            {
                program[pc++] = INT8;
            }
            else if(strcmp(instructions[1],"uint8") == 0)
            {
                program[pc++] = UINT8;
            }
            else if(strcmp(instructions[1],"int16") == 0)
            {
                program[pc++] = INT16;
            }
            else if(strcmp(instructions[1],"uint16") == 0)
            {
                program[pc++] = UINT16;
            }
            else if(strcmp(instructions[1],"int32") == 0)
            {
                program[pc++] = INT32;
            }
            else if(strcmp(instructions[1],"uint32") == 0)
            {
                program[pc++] = UINT32;
            }
            else if(strcmp(instructions[1],"int64") == 0)
            {
                program[pc++] = INT64;
            }
            else if(strcmp(instructions[1],"uint64") == 0)
            {
                program[pc++] = UINT64;
            }
            else if(strcmp(instructions[1],"float32") == 0)
            {
                program[pc++] = FLOAT32;
            }
            else if(strcmp(instructions[1],"float64") == 0)
            {
                program[pc++] = FLOAT64;
            }
            else if(strcmp(instructions[1],"ptr") == 0)
            {
                program[pc++] = POINTER;
            }
            else
            {
                printf("Invalid type on line: %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"storeptr") == 0)
        {
            program[pc++] = (char)StorePtr;
        }

        if(strcmp(instructions[0],"getptr") == 0)
        {
            program[pc++] = (char)GetPtr;
        }

        if(strcmp(instructions[0],"freeptr") == 0)
        {
            program[pc++] = (char)FreePtr;
        }

        if(strcmp(instructions[0],"free") == 0)
        {
            program[pc++] = (char)Free;
        }


        if(strcmp(instructions[0],"add") == 0)
        {
            program[pc++] = (char)Add;
        }

        if(strcmp(instructions[0],"sub") == 0)
        {
            program[pc++] = (char)Sub;
        }

        if(strcmp(instructions[0],"mul") == 0)
        {
            program[pc++] = (char)Mul;
        }

        if(strcmp(instructions[0],"div") == 0)
        {
            program[pc++] = (char)Div;
        }

        if(strcmp(instructions[0],"Convert") == 0)
        {
        }

        if(strcmp(instructions[0],"smallerthan") == 0)
        {
            program[pc++] = (char)SmallerThan;
        }

        if(strcmp(instructions[0],"largerthan") == 0)
        {
            program[pc++] = (char)LargerThan;
        }

        if(strcmp(instructions[0],"equal") == 0)
        {
            program[pc++] = (char)Equal;
        }

        if(strcmp(instructions[0],"if") == 0)
        {
            program[pc++] = (char)If;
            aLabels[aLabelIndex].index = pc;
            aLabels[aLabelIndex].label = malloc(size-2);
            memcpy(aLabels[aLabelIndex].label, instructions[1], size-2);
            aLabels[aLabelIndex].label[size-2] = '\0';
            #if DEBUG
                printf("awaiting label %s at index %i\n", aLabels[aLabelIndex].label, aLabels[aLabelIndex].index);
            #endif
            aLabelIndex++;
            uint64 temp = 0;
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"ifn") == 0)
        {
            program[pc++] = (char)IfNot;
            aLabels[aLabelIndex].index = pc;
            aLabels[aLabelIndex].label = malloc(size-2);
            memcpy(aLabels[aLabelIndex].label, instructions[1], size-2);
            aLabels[aLabelIndex].label[size-2] = '\0';
            #if DEBUG
                printf("awaiting label %s at index %i\n", aLabels[aLabelIndex].label, aLabels[aLabelIndex].index);
            #endif
            aLabelIndex++;
            uint64 temp = 0;
            memcpy(program + pc, &temp, 8);
            pc+=8;
        }

        if(strcmp(instructions[0],"and") == 0)
        {
            program[pc++] = (char)And;
        }

        if(strcmp(instructions[0],"or") == 0)
        {
            program[pc++] = (char)Or;
        }

        if(strcmp(instructions[0],"not") == 0)
        {
            program[pc++] = (char)Not;
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
    for(uint64 i = 0; i < aLabelIndex; i++)
    {
        #if DEBUG
            printf("%s\n",aLabels[i].label);
            printf("%i\n",aLabels[i].index);
        #endif
        for(uint64 j = 0; j < labelIndex; j++)
        {
            #if DEBUG
                printf("\t%s\n",labels[j].label);
                printf("\t%i\n",labels[j].index);
            #endif
            if(strcmp(aLabels[i].label,labels[j].label) == 0)
            {
                #if DEBUG
                    printf("found label\n");
                #endif
                memcpy(program + aLabels[i].index, &labels[j].index, 8);
            }
        }
    }

    //Write to file
    FILE* out = fopen(outFile,"wb");
    fwrite(program, 1, pc, out);
    fclose(out);

    free(program);
    free(labels);
    free(aLabels);
    free(instructions);
}

int main(int argc, const char** argv)
{
    char* in  = argv[1];
    char* out = argv[2];
    assemble(in, out);
    printf("\nDone!\n");
	return 0;
}