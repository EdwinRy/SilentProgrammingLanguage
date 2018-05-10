#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef long long int64;
typedef enum SilentBytecode
{
	Halt,
	Goto,
	Sweep,
    UseGlobal,
    EndGlobal,
	Call,
	Return,

	Push1,
	Push2,
	Push4,
	Push8,
	PushX,
	
	Pop1,
	Pop2,
	Pop4,
	Pop8,
	PopX,

	Store1,
	Store2,
	Store4,
	Store8,
	StoreX,
	
	Load1,
	Load2,
	Load4,
	Load8,
	LoadX,

	Alloc1,
	Alloc2,
	Alloc4,
	Alloc8,
	AllocX,

	LoadPtr1,
	LoadPtr2,
	LoadPtr4,
	LoadPtr8,
	LoadPtrX,

	StorePtr1,
	StorePtr2,
	StorePtr4,
	StorePtr8,
	StorePtrX,

    GetPtr,
	FREE,

	AddByte,
	AddShort,
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	SubByte,
	SubShort,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	MulByte,
	MulShort,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	DivByte,
	DivShort,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	ByteToInt,
	ByteToShort,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	IntToByte,
	IntToShort,
	IntToLong,
	IntToFloat,
	IntToDouble,

	FloatToByte,
	FloatToShort,
	FloatToInt,
	FloatToLong,
	FloatToDouble,

	DoubleToByte,
	DoubleToShort,
	DoubleToInt,
	DoubleToLong,
	DoubleToFloat,

	LongToByte,
	LongToShort,
	LongToInt,
	LongToFloat,
	LongToDouble,
	
	SmallerThanByte,
	SmallerThanShort,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	LargerThanByte,
	LargerThanShort,
	LargerThanInt,
	LargerThanLong,
	LargerThanFloat,
	LargerThanDouble,

	EqualByte,
	EqualShort,
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

typedef struct silentLabel
{
    char* label;
    int index;
}silentLabel;

int getLabelIndex(silentLabel* labels,int count, char* label)
{
    for(int i = 0; i < count; i++)
    {
        if(strcmp(labels[i].label,label)==0)
        {
            return labels[i].index;
        }
       
    }
}
char assemble(char* inFile, char* outFile)
{
    char* program = malloc(1000);
    uint64 programCounter = 0;

    silentLabel labels[1000];
    silentLabel gotos[1000];
    uint labelIndex = 0;
    uint gotosIndex = 0;

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
        if(line[0] == '/')
        {
            currentLine += 1;
            size = getline(&line,&s,f);
            continue;
        }
        char buffer[255];
        char* instructions[3];
        int tempVar = 0;
        int instructionIndex = 0;
        for(int i = 0; i < size;i++)
        {
            //Parse token
            if(isalpha(line[i]))
            {
                //Number of characters
			    unsigned char count = 0;
			    //prepare space for the value
			    char* value;
			    //As long as the current character isn't whitespace
			    while(isalpha(line[i+count]) ||
                    isdigit(line[i+count]) || 
                    line[i+count] == ':')
			    {
				    buffer[count] = line[i+count];
                    count+=1;
			    }
			    //Allocate space for the value and terminator
			    instructions[instructionIndex] = malloc(count+1);
			    //Copy the value from the buffer
			    memcpy(instructions[instructionIndex],buffer,count);
			    instructions[instructionIndex][count] = '\0';
                i+=count;
                instructionIndex+=1;
            }
            //Parse string
            if(line[i] == '\"')
            {
                i+=1;
                int count = 1;
                buffer[0] = 's';
                for(count = 1;line[i]!='\"';count++)
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
                    i+=1;
                }
                //Allocate space for the value and terminator
			    instructions[instructionIndex] = malloc(count);
			    //Copy the value from the buffer
			    memcpy(instructions[instructionIndex],buffer,count);
			    //instructions[instructionIndex][count] = '\0';
                i+=count;
                instructionIndex+=1;
            }
        }
        if(instructions[0][size-2] == ':')
        {
            silentLabel label;
            label.index = programCounter;
            label.label = malloc(size-2);
            memcpy(label.label,instructions[0],size-2);
            label.label[size-2] = '\0';
            labels[labelIndex] = label;
            labelIndex += 1;
        }

        if(strcmp(instructions[0],"halt") == 0)
        {
            program[programCounter] = (char)Halt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"goto") == 0)
        {
            //int temp = (unsigned int)atoi(instructions[1]+1);
            program[programCounter] = (char)Goto;
            programCounter += 1;
            silentLabel go;
            go.index = programCounter;
            go.label = malloc(size);
            memcpy(go.label,instructions[1],size);
            gotos[gotosIndex] = go;
            gotosIndex += 1;    
            programCounter+=sizeof(uint64);
        }


        /*
        if(strcmp(instructions[0],"callsys") == 0)
        {
            program[programCounter] = (char)CallSys;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                char temp = (int)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += 1;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        */

        if(strcmp(instructions[0],"sweep") == 0)
        {
            program[programCounter] = (char)Sweep;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"useglobal") == 0)
        {
            program[programCounter] = (char)UseGlobal;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"endglobal") == 0)
        {
            program[programCounter] = (char)EndGlobal;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"call") == 0)
        {
            program[programCounter] = (char)Call;
            programCounter+=1;
            silentLabel go;
            go.index = programCounter;
            go.label = malloc(size);
            memcpy(go.label,instructions[1],size);
            gotos[gotosIndex] = go;
            gotosIndex += 1;    
            programCounter+=sizeof(uint64);
        }
        
        if(strcmp(instructions[0],"return") == 0)
        {
            program[programCounter] = (char)Return;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"push1") == 0)
        {
            program[programCounter] = (char)Push1;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                char temp = (char)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    1
                );
                programCounter += 1;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"push2") == 0)
        {
            program[programCounter] = (char)Push2;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                short temp = (short)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    2
                );
                programCounter += 1;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }


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
                    4
                );
                programCounter += 4;
            }
            else if(instructions[1][0] == 'd')
            {
                float temp = (float)atof(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    4
                );
                programCounter += 4;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"push8") == 0)
        {
            program[programCounter] = (char)Push8;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else if(instructions[1][0] == 'f')
            {
                double temp = (double)atof(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"pushx") == 0)
        {
            program[programCounter] = (char)PushX;
            programCounter+=1;
            uint64 temp = (uint64)atol(instructions[1]+1);
            if(instructions[1][0] == 'i')
            {
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }

            if(instructions[2][0] == 'i')
            {
                uint64 temp2 = (uint64)atol(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp2,
                    8
                );
                programCounter += 8;
            }
            else if(instructions[2][0] == 'f')
            {
                double temp2 = (double)atof(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp2,
                    8
                );
                programCounter += 8;
            }
            else if(instructions[2][0] == 's')
            {
                memcpy(
                    program + programCounter,
                    instructions[2] + 1,
                    temp
                );
                programCounter += temp;
            }
        }

        if(strcmp(instructions[0],"pop1") == 0)
        {
            program[programCounter] = (char)Pop1;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"pop4") == 0)
        {
            program[programCounter] = (char)Pop4;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"pop8") == 0)
        {
            program[programCounter] = (char)Pop8;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"popx") == 0)
        {
            program[programCounter] = (char)PopX;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"store1") == 0)
        {
            program[programCounter] = (char)Store1;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"store2") == 0)
        {
            program[programCounter] = (char)Store2;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"store4") == 0)
        {
            program[programCounter] = (char)Store4;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"store8") == 0)
        {
            program[programCounter] = (char)Store8;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"storex") == 0)
        {
            program[programCounter] = (char)StoreX;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
            if(instructions[2][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"load1") == 0)
        {
            program[programCounter] = (char)Load1;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"load2") == 0)
        {
            program[programCounter] = (char)Load2;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"load4") == 0)
        {
            program[programCounter] = (char)Load4;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"load8") == 0)
        {
            program[programCounter] = (char)Load8;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"loadx") == 0)
        {
            program[programCounter] = (char)LoadX;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
            if(instructions[2][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }



        if(strcmp(instructions[0],"alloc1") == 0)
        {
            program[programCounter] = (char)Alloc1;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"alloc2") == 0)
        {
            program[programCounter] = (char)Alloc2;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"alloc4") == 0)
        {
            program[programCounter] = (char)Alloc4;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"alloc8") == 0)
        {
            program[programCounter] = (char)Alloc8;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"allocx") == 0)
        {
            program[programCounter] = (char)AllocX;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"loadptr1") == 0)
        {
            program[programCounter] = (char)LoadPtr1;
            programCounter+=1;

        }

        if(strcmp(instructions[0],"loadptr2") == 0)
        {
            program[programCounter] = (char)LoadPtr2;
            programCounter+=1;

        }

        if(strcmp(instructions[0],"loadptr4") == 0)
        {
            program[programCounter] = (char)LoadPtr4;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"loadptr8") == 0)
        {
            program[programCounter] = (char)LoadPtr8;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"loadptrx") == 0)
        {
            program[programCounter] = (char)LoadPtrX;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"storeptr1") == 0)
        {
            program[programCounter] = (char)StorePtr1;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"storeptr2") == 0)
        {
            program[programCounter] = (char)StorePtr2;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"storeptr4") == 0)
        {
            program[programCounter] = (char)StorePtr4;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"storeptr8") == 0)
        {
            program[programCounter] = (char)StorePtr8;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"storeptrx") == 0)
        {
            program[programCounter] = (char)StorePtrX;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }


        if(strcmp(instructions[0],"getptr") == 0)
        {
            program[programCounter] = (char)GetPtr;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"free") == 0)//
        {
            program[programCounter] = (char)FREE;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                uint64 temp = (uint64)atol(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    8
                );
                programCounter += 8;
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }



        if(strcmp(instructions[0],"addbyte") == 0)
        {
            program[programCounter] = (char)AddByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"addshort") == 0)
        {
            program[programCounter] = (char)AddShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"addint") == 0)
        {
            program[programCounter] = (char)AddInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"addlong") == 0)
        {
            program[programCounter] = (char)AddLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"addfloat") == 0)
        {
            program[programCounter] = (char)AddFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"adddouble") == 0)
        {
            program[programCounter] = (char)AddDouble;
            programCounter+=1;
        }

        
        
        if(strcmp(instructions[0],"subbyte") == 0)
        {
            program[programCounter] = (char)SubByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"subshort") == 0)
        {
            program[programCounter] = (char)SubShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"subint") == 0)
        {
            program[programCounter] = (char)SubInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"sublong") == 0)
        {
            program[programCounter] = (char)SubLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"subfloat") == 0)
        {
            program[programCounter] = (char)SubFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"subdouble") == 0)
        {
            program[programCounter] = (char)SubDouble;
            programCounter+=1;
        }
        
        
        
        if(strcmp(instructions[0],"mulbyte") == 0)
        {
            program[programCounter] = (char)MulByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"mulshort") == 0)
        {
            program[programCounter] = (char)MulShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"mulint") == 0)
        {
            program[programCounter] = (char)MulInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"mullong") == 0)
        {
            program[programCounter] = (char)MulLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"mulfloat") == 0)
        {
            program[programCounter] = (char)MulFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"muldouble") == 0)
        {
            program[programCounter] = (char)MulDouble;
            programCounter+=1;
        }

        
        
        if(strcmp(instructions[0],"divbyte") == 0)
        {
            program[programCounter] = (char)DivByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"divshort") == 0)
        {
            program[programCounter] = (char)DivShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"divint") == 0)
        {
            program[programCounter] = (char)DivInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"divlong") == 0)
        {
            program[programCounter] = (char)DivLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"divfloat") == 0)
        {
            program[programCounter] = (char)DivFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"divdouble") == 0)
        {
            program[programCounter] = (char)DivDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"bytetoint") == 0)
        {
            program[programCounter] = (char)ByteToInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"bytetoshort") == 0)
        {
            program[programCounter] = (char)ByteToShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"bytetolong") == 0)
        {
            program[programCounter] = (char)ByteToLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"bytetofloat") == 0)
        {
            program[programCounter] = (char)ByteToFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"bytetodouble") == 0)
        {
            program[programCounter] = (char)ByteToDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"inttobyte") == 0)
        {
            program[programCounter] = (char)IntToByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"inttoshort") == 0)
        {
            program[programCounter] = (char)IntToShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"inttolong") == 0)
        {
            program[programCounter] = (char)IntToLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"inttofloat") == 0)
        {
            program[programCounter] = (char)IntToFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"inttodouble") == 0)
        {
            program[programCounter] = (char)IntToDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"floattobyte") == 0)
        {
            program[programCounter] = (char)FloatToByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"floattoshort") == 0)
        {
            program[programCounter] = (char)FloatToShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"floattoint") == 0)
        {
            program[programCounter] = (char)FloatToInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"floattolong") == 0)
        {
            program[programCounter] = (char)FloatToLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"floattodouble") == 0)
        {
            program[programCounter] = (char)FloatToDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"doubletobyte") == 0)
        {
            program[programCounter] = (char)DoubleToByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"doubletoshort") == 0)
        {
            program[programCounter] = (char)DoubleToShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"doubletoint") == 0)
        {
            program[programCounter] = (char)DoubleToInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"doubletolong") == 0)
        {
            program[programCounter] = (char)DoubleToLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"doubletofloat") == 0)
        {
            program[programCounter] = (char)DoubleToFloat;
            programCounter+=1;
        }


        if(strcmp(instructions[0],"longtobyte") == 0)
        {
            program[programCounter] = (char)LongToByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"longtoshort") == 0)
        {
            program[programCounter] = (char)LongToShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"longtoint") == 0)
        {
            program[programCounter] = (char)LongToInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"longtofloat") == 0)
        {
            program[programCounter] = (char)LongToFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"longtodouble") == 0)
        {
            program[programCounter] = (char)LongToDouble;
            programCounter+=1;
        }


        if(strcmp(instructions[0],"smallerthanbyte") == 0)
        {
            program[programCounter] = (char)SmallerThanByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"smallerthanshort") == 0)
        {
            program[programCounter] = (char)SmallerThanShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"smallerthanint") == 0)
        {
            program[programCounter] = (char)SmallerThanInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"smallerthanlong") == 0)
        {
            program[programCounter] = (char)SmallerThanLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"smallerthanfloat") == 0)
        {
            program[programCounter] = (char)SmallerThanFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"smallerthandouble") == 0)
        {
            program[programCounter] = (char)SmallerThanDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"largerthanbyte") == 0)
        {
            program[programCounter] = (char)LargerThanByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanshort") == 0)
        {
            program[programCounter] = (char)LargerThanShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanint") == 0)
        {
            program[programCounter] = (char)LargerThanInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanlong") == 0)
        {
            program[programCounter] = (char)LargerThanLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanfloat") == 0)
        {
            program[programCounter] = (char)LargerThanFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthandouble") == 0)
        {
            program[programCounter] = (char)LargerThanDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"equalbyte") == 0)
        {
            program[programCounter] = (char)EqualByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"equalshort") == 0)
        {
            program[programCounter] = (char)EqualShort;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"equalint") == 0)
        {
            program[programCounter] = (char)EqualInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"equallong") == 0)
        {
            program[programCounter] = (char)EqualLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"equalfloat") == 0)
        {
            program[programCounter] = (char)EqualFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"equaldouble") == 0)
        {
            program[programCounter] = (char)EqualDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"if") == 0)
        {
            program[programCounter] = (char)If;
            programCounter += 1;
            silentLabel go;
            go.index = programCounter;
            go.label = malloc(size);
            memcpy(go.label,instructions[1],size);
            gotos[gotosIndex] = go;
            gotosIndex += 1;    
            programCounter += 8;
        }

        if(strcmp(instructions[0],"ifnot") == 0)
        {
            program[programCounter] = (char)IfNot;
            programCounter+=1;
            silentLabel go;
            go.index = programCounter;
            go.label = malloc(size);
            memcpy(go.label,instructions[1],size);
            gotos[gotosIndex] = go;
            gotosIndex += 1;    
            programCounter+=8;
        }

        currentLine += 1;
        size = getline(&line,&s,f);
        for(int i = 0; i < instructionIndex; i++)
        {
            free(instructions[i]);
        }
        instructionIndex = 0;
    }

    //Sort out gotos
    for(int i = 0; i < gotosIndex; i++)
    {
        int index = getLabelIndex(labels,labelIndex,gotos[i].label);
        memcpy(
            program + gotos[i].index,
            &index,
            8
        );
    }

    //Write to file
    FILE* out = fopen(outFile,"wb");
    fwrite(program,1,programCounter,out);
    fclose(out);
    return 1;
}

int main(int argc, char** argv)
{
    char* in  = argv[1];
    char* out = argv[2];
    assemble(in, out);
}