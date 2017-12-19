#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef enum SilentBytecode
{
	Halt,
	Goto, //X - Byte
	Call,//
	
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
	Store1, //X - memory adress
	Store4, //X - memory adress
	Store8, //X - memory adress
	StoreX, //X - X value size, X - memory adress
	
	//Load value from memory
	Load1, //X - memory adress
	Load4, //X - memory adress
	Load8, //X - memory adress
	LoadX, //X - X value size, X - memory adress

	//Allocate space for a value
	Alloc1, //X - memory adress
	Alloc4, //X - memory adress
	Alloc8, //X - memory adress
	AllocX, //X - memory adress

	//Free a value at adress
	FREE, //X - memory adress

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
    for(int i = 0; i < count; i++)
    {
        if(strcmp(labels[i].label,label)==0)
        {
            //printf("kek %s\n",labels[i].label);
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
        if(instructions[0][size-2] == ':')
        {
            //printf("label %s\n",instructions[0]);
            silentLabel label;
            label.index = programCounter;
            label.label = malloc(size-1);
            memcpy(label.label,instructions[0],size-2);
            label.label[size-1] = '\0';
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