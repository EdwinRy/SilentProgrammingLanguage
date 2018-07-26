#include "SilentVM2.h"
#include "SilentLib.h"
#include <stdio.h>
char* readAllText(char* path)
{
    char* text;
	FILE *f;
	if((f = fopen(path,"r"))==NULL)
	{
		printf("File %s doesn't exist!\n",path);
		exit(-1);
	}
	fseek(f,0,SEEK_END);
	text = malloc(ftell(f));
	fseek(f,0,SEEK_SET);
	long count = 0;
	char c;
	while((c = fgetc(f))!=EOF){text[count++] = (char)c;}
    fclose(f);
    return text;
}

int main(int argc, char** argv)
{
	//if(argc<2){printf("SilentVM requires an input file\n");return(-1);}
    //char* inFile = argv[1];
    //char* program = readAllText(inFile); 

	//random float value in binary for testing: 102,102,6,64 = 2.1
	//random double value in binary: -52,-52,-52,-52,-52,-52,0,64 = 2.1

	/*
	char program[] = 
	{
		//Go to main
		Goto,17,0,0,0,0,0,0,0, //+9 (0)
		//Add 10 Sub
		Push,5,10,0,0,0, 	//+6 (9)
		Add,				//+1 (15)
		Return,				//+1 (16)
		//Main
		Push,5,9,0,0,0,		//+6 (17)
		Call, 9,0,0,0,0,0,0,0, 4,0,0,0,0,0,0,0,
		Push,5,11,0,0,0,
		Add,
		Halt
	};*/


	if(argc < 2)
	{
		printf("Usage: SilentVM <file name>\n");
		exit(-1);
	}
	char* program = readAllText(argv[1]);

	SilentMemory* memory = createSilentMemory(8000,8000);
	SilentGC* gc = createSilentGC(memory);
	SilentVM* vm = createSilentVM(memory,program,gc);

	silentVMStart(vm);

	deleteSilentGC(gc);
	deleteSilentMemory(memory);
	//deleteSilentVM(vm);
	

    return 0;
}