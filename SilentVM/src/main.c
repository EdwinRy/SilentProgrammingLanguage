#include "SilentVM.h"
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
	
	char program[] = 
	{
		//Push8,
		//-51,-52,-52,-52,-52,-52,0,64,
		Halt
	};

	char fib[] = 
	{
		//var n = 40 //index = 0
		Push4, 40,0,0,0,
		//i = 2 //index = 4
		Push4, 2,0,0,0,
		//fn = 0 //index = 8
		Push4, 0,0,0,0,
		//f1 = 0 //index = 12
		Push4, 0,0,0,0,
		//f2 = 2 //index = 16
		Push4, 1,0,0,0,
		//For location = 25
		Load4, 0,0,0,0,0,0,0,0,
		Load4, 4,0,0,0,0,0,0,0, //43

		SmallerThanInt, //44
		If, 127,0,0,0,0,0,0,0, //Goto end //53
		Load4, 12,0,0,0,0,0,0,0, //62
		Load4, 16,0,0,0,0,0,0,0, //71
		AddInt, //72
		Store4, 8,0,0,0,0,0,0,0, //81

		Load4, 16,0,0,0,0,0,0,0, //90
		Load4, 8,0,0,0,0,0,0,0, //99

		Store4, 16,0,0,0,0,0,0,0, //108
		Store4, 12,0,0,0,0,0,0,0, //117

		Goto, 25,0,0,0,0,0,0,0, //126

		Halt, //127
	};

	SilentMemory* memory = createSilentMemory(8000,8000);
	SilentGC* gc = createSilentGC(memory);
	SilentVM* vm = createSilentVM(memory,fib,gc);

	silentVMStart(vm);

	printf("stack %i\n",*((char*)(memory->stack+4)));
	printf("stack ptr %i\n",memory->stackPointer);
	printf("stack T %i\n",memory->stackTypes->data[0]);
	printf("stack T c %i\n",memory->stackTypes->ptr);

	deleteSilentGC(gc);
	deleteSilentMemory(memory);
	//deleteSilentVM(vm);
	

    return 0;
}