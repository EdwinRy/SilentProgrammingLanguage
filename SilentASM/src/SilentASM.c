#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef enum SilentBytecode
{
	Halt,
	Goto,
	Call,
	
	Push1,
	Push4,
	Push8,
	PushX,
	
	Pop1,
	Pop4,
	Pop8,
	PopX,

	Store1,
	Store4,
	Store8,
	StoreX,
	
	Load1,
	Load4,
	Load8,
	LoadX,

	Alloc1,
	Alloc4,
	Alloc8,
	AllocX,

	FREE,	

	/*
	LoadPtr,
	EditPtr1,
	EditPtr4,
	EditPtr8,
	EditPtrX,
	*/

	AddByte,	
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	SubByte,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	MulByte,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	DivByte,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	ByteToInt,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	IntToByte,
	IntToLong,
	IntToFloat,
	IntToDouble,

	FloatToByte,
	FloatToInt,
	FloatToLong,
	FloatToDouble,

	DoubleToByte,
	DoubleToInt,
	DoubleToLong,
	DoubleToFloat,
	
	SmallerThanByte,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	BiggerThanByte,
	BiggerThanInt,
	BiggerThanLong,
	BiggerThanFloat,
	BiggerThanDouble,

	EqualByte,
	EqualInt,
	EqualLong,
	EqualFloat,
	EqualDouble,
	
	If,
	IfNot
}SilentBytecode;

typedef struct silentInstruction
{
    char* data;
}silentInstruction;

char assemble(char* inFile, char* outFile)
{
    char* program = malloc(1000);
    unsigned int programCounter = 0;
    FILE* f;
    f = fopen(inFile,"r");
    if(f==NULL)
    {
        return 0;
        printf("file %s could not be opened\n", inFile);
    }

    char* line;
    int currentLine = 0;
    size_t s = 0;
    int size = 0;
    size = getline(&line,&s,f);

    while(size != -1)
    {
        char buffer[255];
        char* instructions[3];
        int instructionIndex = 0;
        for(int i = 0; i < size;i++)
        {
            if(isalpha(line[i]))
            {
                //printf("here\n");
                //Number of characters
			    unsigned char count = 0;
			    //prepare space for the value
			    char* value;
			    //As long as the current character isn't whitespace
			    while(isalpha(line[i+count]) ||
                    isdigit(line[i+count]) || 
                    line[i+count] == ':')
			    {
				    //Assign buffer to the current character
				    buffer[count++] = line[i+count];
			    }
			    //Allocate space for the value and terminator
			    instructions[instructionIndex] = malloc(count+1);
			    //Copy the value from the buffer
			    memcpy(instructions[instructionIndex],buffer,count);
			    instructions[instructionIndex][count] = '\0';
                //printf("%s\n",instructions[instructionIndex]);
                i+=count;
                instructionIndex+=1;
                //printf("here2\n");
                
            }
        }

        if(instructions[0][size] == ':')
        {
            printf("label %s\n",instructions[0]);
        }

        if(strcmp(instructions[0],"halt") == 0)
        {
            program[programCounter] = (char)Halt;
            programCounter+=1;
            printf("halt\n");
        }
        if(strcmp(instructions[0],"goto") == 0)
        {
            int temp = (unsigned int)atoi(instructions[1]+1);
            program[programCounter] = (char)Goto;
            programCounter+=1;
            memcpy(
                program + programCounter,
                &temp,
                sizeof(int)
            );
            programCounter+=sizeof(int);
            printf("goto\n");
        }
        /*
        if(strcmp(instructions[0],"call"))
        {
            program[programCounter] = (char)Halt;
            programCounter+=1;
        }*/
        if(strcmp(instructions[0],"push4") == 0)
        {
            program[programCounter] = (char)Push4;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                int temp = (int)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
            }
            else if(instructions[1][0] == 'd')
            {
                float temp = (float)atof(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(float)
                );
                programCounter += sizeof(float);
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
            printf("push4\n");
        }
        currentLine += 1;
        size = getline(&line,&s,f);
        for(int i = 0; i < instructionIndex; i++)
        {
            free(instructions[i]);
        }
        instructionIndex = 0;
    }


    //Write to file
    FILE* out = fopen(outFile,"wb");
    fwrite(program,1,programCounter,out);
    fclose(out);
    return 1;
}

int main(int argc, char** argv)
{
    char* in = argv[1];
    char* out =argv[2];
    assemble(in, out);
}