#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//<instruction> <argument> <argument>
//Argument types:
//X = number
//Y = string
typedef enum SilentBytecode
{
	//Stops the execution of the program
	Halt,
	//Moves the program counter to X
	Goto, //X - Byte
	//Call native subroutine
	CallSys,//X - 4 bytes

    UseGlobal,
    EndGlobal,

	//Call silent subrouting
	Call,//X - 4 bytes
	//Return back from subroutine
	Return,
	
	//ClearMemory,
	//ClearStack,
	
	//Pushes values onto the stack (CPU stack)
	Push1, //X - 1 byte value
	Push4, //X - 4 byte value
	Push8, //X - 8 byte value
	PushX, //X - X byte value, X - value
	
	//Pops values from the stack
	Pop1, //X - 1 byte value
	Pop4, //X - 4 byte value
	Pop8, //X - 8 byte value
	PopX, //X - X byte value, X - value

	//Store value in dedicated memory
	Store1, //X - memory address
	Store4, //X - memory address
	Store8, //X - memory address
	StoreX, //X - X value size, X - memory address
	
	//Load value from memory
	Load1, //X - memory address
	Load4, //X - memory address
	Load8, //X - memory address
	LoadX, //X - X value size, X - memory address

	//Allocate space for a value
	Alloc1, //X - memory address
	Alloc4, //X - memory address
	Alloc8, //X - memory address
	AllocX, //X - memory address

	//Return a pointer to X and push it to stack
	GetPtr, //X - memory address

	//Push 1 byte from the pointer
	LoadPtr1, 
	//Push 4 byte from the pointer
	LoadPtr4, 
	//Push 8 byte from the pointer
	LoadPtr8, 
	//Push X bytes from the pointer
	LoadPtrX, //X - value size

	//Change 1 byte at a pointer
	EditPtr1,
	//Change 4 bytes at a pointer
	EditPtr4,
	//Change 8 bytes at a pointer
	EditPtr8,
	//Change X bytes at a pointer
	EditPtrX, //X - value size

	//Free a value at address
	FREE, //X - memory address

	//Add numbers on the stack together
	AddByte,
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	//Subtract values on the stack
	//stack before:    	stack after
	// 2				0
	// 4				2
	//Operation is applied to the first
	//pushed value
	SubByte,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	//Multiply numbers on the stack
	MulByte,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	//Divide numbers on the stack
	//in the same order as subtraction
	DivByte,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	//Casting bytes to other data
	ByteToInt,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	//Casting integers to other data
	IntToByte,
	IntToLong,
	IntToFloat,
	IntToDouble,

	//Casting floats to other data
	FloatToByte,//
	FloatToInt,//
	FloatToLong,//
	FloatToDouble,//

	//Casting doubles to other data
	DoubleToByte,//
	DoubleToInt,//
	DoubleToLong,//
	DoubleToFloat,//

	//Cast long value to other data types
	LongToByte,//
	LongToInt,//
	LongToFloat,//
	LongToDouble,//
	
	//Smaller than operators
	//Same order as with subtraction
	SmallerThanByte,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	//Larger than operators
	//Same order as subtraction
	BiggerThanByte,
	BiggerThanInt,
	BiggerThanLong,
	BiggerThanFloat,
	BiggerThanDouble,

	//Equality operators
	EqualByte,
	EqualInt,
	EqualLong,
	EqualFloat,
	EqualDouble,

	//All of the comparison operators return either
	// a 1 - true or 0 - false to the stack
	
	//If theres a 1 on the stack
	If,
	//If the isn't one
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
    //printf("label %s\n",label);
    for(int i = 0; i < count; i++)
    {
       // printf("label test:%s\n",labels[i].label);
        if(strcmp(labels[i].label,label)==0)
        {
            //printf("label2 %s\n",labels[i].label);
            return labels[i].index;
        }
       
    }
}
char assemble(char* inFile, char* outFile)
{
    char* program = malloc(1000);
    unsigned int programCounter = 0;
    silentLabel labels[1000];
    silentLabel gotos[1000];
    unsigned int labelIndex = 0;
    unsigned int gotosIndex = 0;
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
            programCounter+=sizeof(int);
        }



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
            programCounter+=sizeof(int);
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
                    sizeof(char)
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
        }
        if(strcmp(instructions[0],"push8") == 0)
        {
            program[programCounter] = (char)Push8;
            programCounter+=1;
            if(instructions[1][0] == 'i')
            {
                long temp = (long)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(long)
                );
                programCounter += sizeof(long);
            }
            else if(instructions[1][0] == 'd')
            {
                double temp = (double)atof(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(double)
                );
                programCounter += sizeof(double);
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
            int temp = (int)atoi(instructions[1]+1);
            if(instructions[1][0] == 'i')
            {
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }

            if(instructions[2][0] == 'i')
            {
                int temp2 = (int)atoi(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp2,
                    sizeof(int)
                );
                programCounter += sizeof(int);
            }
            else if(instructions[2][0] == 'f')
            {
                int temp2 = (float)atof(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp2,
                    sizeof(float)
                );
                programCounter += sizeof(float);
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
        if(strcmp(instructions[0],"popx") == 0)//
        {
            program[programCounter] = (char)PopX;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }



        if(strcmp(instructions[0],"store1") == 0)//
        {
            program[programCounter] = (char)Store1;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"store4") == 0)//
        {
            program[programCounter] = (char)Store4;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"store8") == 0)//
        {
            program[programCounter] = (char)Store8;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"storex") == 0)//
        {
            program[programCounter] = (char)StoreX;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
            if(instructions[2][0] == 'i')
            {
                int temp = (int)atoi(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }



        if(strcmp(instructions[0],"load1") == 0)//
        {
            program[programCounter] = (char)Load1;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"load4") == 0)//
        {
            program[programCounter] = (char)Load4;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"load8") == 0)//
        {
            program[programCounter] = (char)Load8;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"loadx") == 0)//
        {
            program[programCounter] = (char)LoadX;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
            if(instructions[2][0] == 'i')
            {
                int temp = (int)atoi(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }



        if(strcmp(instructions[0],"alloc1") == 0)//
        {
            program[programCounter] = (char)Alloc1;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"alloc4") == 0)//
        {
            program[programCounter] = (char)Alloc4;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"alloc8") == 0)//
        {
            program[programCounter] = (char)Alloc8;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }
        if(strcmp(instructions[0],"allocx") == 0)//
        {
            program[programCounter] = (char)AllocX;
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
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
            if(instructions[2][0] == 'i')
            {
                int temp = (int)atoi(instructions[2]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
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
                int temp = (int)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
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
                int temp = (int)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
            }
            else
            {
                printf("Use of incorrect type on line %i\n",currentLine);
            }
        }

        if(strcmp(instructions[0],"editptr1") == 0)
        {
            program[programCounter] = (char)EditPtr1;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"editptr4") == 0)
        {
            program[programCounter] = (char)EditPtr4;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"editptr8") == 0)
        {
            program[programCounter] = (char)EditPtr8;
            programCounter+=1;
        }

        if(strcmp(instructions[0],"editptrx") == 0)
        {
            program[programCounter] = (char)EditPtrX;
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
                int temp = (int)atoi(instructions[1]+1);
                memcpy(
                    program + programCounter,
                    &temp,
                    sizeof(int)
                );
                programCounter += sizeof(int);
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
            program[programCounter] = (char)BiggerThanByte;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanint") == 0)
        {
            program[programCounter] = (char)BiggerThanInt;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanlong") == 0)
        {
            program[programCounter] = (char)BiggerThanLong;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthanfloat") == 0)
        {
            program[programCounter] = (char)BiggerThanFloat;
            programCounter+=1;
        }
        if(strcmp(instructions[0],"largerthandouble") == 0)
        {
            program[programCounter] = (char)BiggerThanDouble;
            programCounter+=1;
        }



        if(strcmp(instructions[0],"equalbyte") == 0)
        {
            program[programCounter] = (char)EqualByte;
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
            programCounter+=1;
            silentLabel go;
            go.index = programCounter;
            go.label = malloc(size);
            memcpy(go.label,instructions[1],size);
            gotos[gotosIndex] = go;
            gotosIndex += 1;    
            programCounter+=sizeof(int);
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
            programCounter+=sizeof(int);
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
        int index = 
            getLabelIndex(
                labels,
                labelIndex,
                gotos[i].label
                );
        //printf("%i\n",index);
        memcpy(
            program + gotos[i].index,
            &index,
            sizeof(int)
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