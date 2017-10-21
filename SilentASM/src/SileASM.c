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
	FlaotToLong,
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
	program = fopen(outputPath,"wb");
	for(int i = 0; i < linesCount; i++)
	{
		if(strcmp(source[i], "halt") == 0)
		{
			fprintf(program,"%c","0");
		}
		if(source[i][0] == ":")
		{
			continue;
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
