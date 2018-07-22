#include <stdlib.h>
#include <stdio.h>
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



char* outputPath;
char* currentDir;
char** source;
long linesCount = 0;
long labelCount = 0;

char** readFileLines(char* path)
{
	//read file
	FILE *fp;
	char *line = malloc(50);
	char **source;
	long len = 0;
	ssize_t read;
	char temp;
	if((fp = fopen(path,"r"))==NULL)
	{
		printf("file %s doesn't exist!",path);
	}

	while((read = fscanf(fp,"%[^\n]\n",line))!= EOF)
	{
		len++;
	}
	rewind(fp);
	source = malloc(sizeof(char*) * len);

	while((read = fscanf(fp,"%[^\n]\n",line))!= EOF)
	{
		source[linesCount] = malloc((unsigned)strlen(line));
		memcpy(source[linesCount],line,strlen(line));
		if(source[linesCount][0] == *":")labelCount++;
		linesCount++;
	}

	fclose(fp);

	return source;
}

char** linkFiles(char** paths, int count)
{

}

int linkLibraries()
{

}

int assembleFile(char* path)
{
	//read file
	source = readFileLines(path);
	long* labelPos = malloc(sizeof(long)*labelCount);
	char** labelNames = malloc(sizeof(char*)*labelCount);

	//scan for labels
	long labCount = 0;
	for(long i = 0; i < linesCount; i++)
	{
		if(source[i][0] == *":")
		{
			labelPos[labCount] = i;
			labelNames[labCount] = malloc(strlen(source[i])-1);
			strcpy(labelNames[labCount],source[i]+1);
			labCount++;
		}
	}

	//Compile
	FILE *program;
	program = fopen(outputPath,"w");
	for(int i = 0; i < linesCount; i++)
	{
		if(source[i][0] == ":")
		{
			continue;
		}

		//Stop program's execution
		else if(strcmp(source[i], "halt") == 0)
		{
			fprintf(program,"%c",(char)Halt);
		}

		//Goto instruction
		else if(strcmp(source[i], "goto") == 0)
		{
			fprintf(program,"%c",(char)Goto);
		}

		//Push values on the stack
		
		else if(strcmp(source[i], "push1") == 0)
		{
			fprintf(program,"%c",(char)Push1);
		}

		else if(strcmp(source[i], "push4") == 0)
		{
			fprintf(program,"%c",(char)Push4);
		}

		else if(strcmp(source[i], "push8") == 0)
		{
			fprintf(program,"%c",(char)Push8);
		}

		else if(strcmp(source[i], "pushX") == 0)
		{
			fprintf(program,"%c",(char)PushX);
		}

		//Remove values from the stack

		else if(strcmp(source[i], "pop1") == 0)
		{
			fprintf(program,"%c",(char)Pop1);
		}

		else if(strcmp(source[i], "pop4") == 0)
		{
			fprintf(program,"%c",(char)Pop4);
		}

		else if(strcmp(source[i], "pop8") == 0)
		{
			fprintf(program,"%c",(char)Pop8);
		}

		else if(strcmp(source[i], "popx") == 0)
		{
			fprintf(program,"%c",(char)PopX);
		}

		//Store values in program's memory

		else if(strcmp(source[i], "store1") == 0)
		{
			fprintf(program,"%c",(char)Store1);
		}

		else if(strcmp(source[i], "store4") == 0)
		{
			fprintf(program,"%c",(char)Store4);
		}

		else if(strcmp(source[i], "store8") == 0)
		{
			fprintf(program,"%c",(char)Store8);
		}

		else if(strcmp(source[i], "storex") == 0)
		{
			fprintf(program,"%c",(char)StoreX);
		}

		//Load values from program's memory onto stack

		else if(strcmp(source[i], "load1") == 0)
		{
			fprintf(program,"%c",(char)Load1);
		}

		else if(strcmp(source[i], "load4") == 0)
		{
			fprintf(program,"%c",(char)Load4);
		}

		else if(strcmp(source[i], "load8") == 0)
		{
			fprintf(program,"%c",(char)Load8);
		}

		else if(strcmp(source[i], "loadx") == 0)
		{
			fprintf(program,"%c",(char)LoadX);
		}

		//Allocate memory for values

		else if(strcmp(source[i], "alloc1") == 0)
		{
			fprintf(program,"%c",(char)Alloc1);
		}

		else if(strcmp(source[i], "alloc4") == 0)
		{
			fprintf(program,"%c",(char)Alloc4);
		}

		else if(strcmp(source[i], "alloc8") == 0)
		{
			fprintf(program,"%c",(char)Alloc8);
		}

		else if(strcmp(source[i], "allocx") == 0)
		{
			fprintf(program,"%c",(char)AllocX);
		}

		//Free allocated memory

		else if(strcmp(source[i], "free") == 0)
		{
			fprintf(program,"%c",FREE);
		}

		//Add values

		else if(strcmp(source[i], "addbyte") == 0)
		{
			fprintf(program,"%c",(char)AddByte);
		}

		else if(strcmp(source[i], "addint") == 0)
		{
			fprintf(program,"%c",(char)AddInt);
		}

		else if(strcmp(source[i], "addlong") == 0)
		{
			fprintf(program,"%c",(char)AddLong);
		}

		else if(strcmp(source[i], "addfloat") == 0)
		{
			fprintf(program,"%c",(char)AddFloat);
		}

		else if(strcmp(source[i], "adddouble") == 0)
		{
			fprintf(program,"%c",(char)AddDouble);
		}

		//Subtract values

		else if(strcmp(source[i], "subbyte") == 0)
		{
			fprintf(program,"%c",(char)SubByte);
		}

		else if(strcmp(source[i], "subint") == 0)
		{
			fprintf(program,"%c",(char)SubInt);
		}

		else if(strcmp(source[i], "sublong") == 0)
		{
			fprintf(program,"%c",(char)SubLong);
		}

		else if(strcmp(source[i], "subfloat") == 0)
		{
			fprintf(program,"%c",(char)SubFloat);
		}

		else if(strcmp(source[i], "subdouble") == 0)
		{
			fprintf(program,"%c",(char)SubDouble);
		}

		//Multiply values

		else if(strcmp(source[i], "mulbyte") == 0)
		{
			fprintf(program,"%c",(char)MulByte);
		}

		else if(strcmp(source[i], "mulint") == 0)
		{
			fprintf(program,"%c",(char)MulInt);
		}

		else if(strcmp(source[i], "mullong") == 0)
		{
			fprintf(program,"%c",(char)MulLong);
		}

		else if(strcmp(source[i], "mulfloat") == 0)
		{
			fprintf(program,"%c",(char)MulFloat);
		}

		else if(strcmp(source[i], "muldouble") == 0)
		{
			fprintf(program,"%c",(char)MulDouble);
		}

		//Divide values

		else if(strcmp(source[i], "divbyte") == 0)
		{
			fprintf(program,"%c",(char)DivByte);
		}

		else if(strcmp(source[i], "divint") == 0)
		{
			fprintf(program,"%c",(char)DivInt);
		}

		else if(strcmp(source[i], "divlong") == 0)
		{
			fprintf(program,"%c",(char)DivLong);
		}

		else if(strcmp(source[i], "divfloat") == 0)
		{
			fprintf(program,"%c",(char)DivFloat);
		}

		else if(strcmp(source[i], "divdouble") == 0)
		{
			fprintf(program,"%c",(char)DivDouble);
		}

		//Type conversions

		else if(strcmp(source[i], "byte2int") == 0)
		{
			fprintf(program,"%c",(char)ByteToInt);
		}

		else if(strcmp(source[i], "byte2long") == 0)
		{
			fprintf(program,"%c",(char)ByteToLong);
		}

		else if(strcmp(source[i], "byte2float") == 0)
		{
			fprintf(program,"%c",(char)ByteToFloat);
		}

		else if(strcmp(source[i], "byte2double") == 0)
		{
			fprintf(program,"%c",(char)ByteToDouble);
		}

		else if(strcmp(source[i], "int2byte") == 0)
		{
			fprintf(program,"%c",(char)IntToByte);
		}

		else if(strcmp(source[i], "int2long") == 0)
		{
			fprintf(program,"%c",(char)IntToLong);
		}

		else if(strcmp(source[i], "int2float") == 0)
		{
			fprintf(program,"%c",(char)IntToFloat);
		}

		else if(strcmp(source[i], "int2double") == 0)
		{
			fprintf(program,"%c",(char)IntToDouble);
		}

		else if(strcmp(source[i], "float2byte") == 0)
		{
			fprintf(program,"%c",(char)FloatToByte);
		}

		else if(strcmp(source[i], "float2int") == 0)
		{
			fprintf(program,"%c",(char)FloatToInt);
		}

		else if(strcmp(source[i], "float2long") == 0)
		{
			fprintf(program,"%c",(char)FloatToLong);
		}

		else if(strcmp(source[i], "float2double") == 0)
		{
			fprintf(program,"%c",(char)FloatToDouble);
		}

		else if(strcmp(source[i], "double2byte") == 0)
		{
			fprintf(program,"%c",(char)DoubleToByte);
		}

		else if(strcmp(source[i], "double2int") == 0)
		{
			fprintf(program,"%c",(char)DoubleToInt);
		}

		else if(strcmp(source[i], "double2long") == 0)
		{
			fprintf(program,"%c",(char)DoubleToLong);
		}

		else if(strcmp(source[i], "double2float") == 0)
		{
			fprintf(program,"%c",(char)DoubleToFloat);
		}

		//Comparison

		else if(strcmp(source[i], "smallerbyte") == 0)
		{
			fprintf(program,"%c",(char)SmallerThanByte);
		}

		else if(strcmp(source[i], "smallerint") == 0)
		{
			fprintf(program,"%c",(char)SmallerThanInt);
		}

		else if(strcmp(source[i], "smallerlong") == 0)
		{
			fprintf(program,"%c",(char)SmallerThanLong);
		}

		else if(strcmp(source[i], "smallerfloat") == 0)
		{
			fprintf(program,"%c",(char)SmallerThanFloat);
		}

		else if(strcmp(source[i], "smallerdouble") == 0)
		{
			fprintf(program,"%c",(char)SmallerThanDouble);
		}

		else if(strcmp(source[i], "biggerbyte") == 0)
		{
			fprintf(program,"%c",(char)BiggerThanByte);
		}

		else if(strcmp(source[i], "biggerint") == 0)
		{
			fprintf(program,"%c",(char)BiggerThanInt);
		}

		else if(strcmp(source[i], "biggerlong") == 0)
		{
			fprintf(program,"%c",(char)BiggerThanLong);
		}

		else if(strcmp(source[i], "biggerfloat") == 0)
		{
			fprintf(program,"%c",(char)BiggerThanFloat);
		}

		else if(strcmp(source[i], "biggerdouble") == 0)
		{
			fprintf(program,"%c",(char)BiggerThanDouble);
		}

		else if(strcmp(source[i], "equalbyte") == 0)
		{
			fprintf(program,"%c",(char)EqualByte);
		}

		else if(strcmp(source[i], "equalint") == 0)
		{
			fprintf(program,"%c",(char)EqualInt);
		}

		else if(strcmp(source[i], "equallong") == 0)
		{
			fprintf(program,"%c",(char)EqualLong);
		}

		else if(strcmp(source[i], "equalfloat") == 0)
		{
			fprintf(program,"%c",(char)EqualFloat);
		}

		else if(strcmp(source[i], "equaldouble") == 0)
		{
			fprintf(program,"%c",(char)EqualDouble);
		}

		else if(strcmp(source[i], "if") == 0)
		{
			fprintf(program,"%c",(char)If);
		}

		else if(strcmp(source[i], "ifnot") == 0)
		{
			fprintf(program,"%c",(char)IfNot);
		}
	}
	fclose(program);
}


int main(int argc, const char** argv)
{
	//currentDir = argv[0];
	char* inPath = NULL;
	if(argc == 1)
	{
		printf("No arguments passed\n");
		printf("type -h to display help\n");
	}
	for(int i = 1; i < argc; i++)
	{
		if(strcmp("-c",argv[i]) == 0)
		{
			inPath = argv[++i];
		}

		else if(strcmp("-o",argv[i]) == 0)
		{
			outputPath = argv[++i];
		}

		else if(strcmp("-h",argv[i]) == 0)
		{
			printf("Type -c <filename> to compile a single file.\n");
			printf("Type -cl <files> to link the files together \n");
			printf("and compile them, they will be linked in order.\n");
			printf("use -o <output directory> to set the output directory\n");
		}
	}

	if(inPath != NULL)
	{
		assembleFile(inPath);
	}

	return 0;
}
