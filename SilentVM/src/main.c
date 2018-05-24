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
		Alloc4,
		GetPtr,0,0,0,0,0,0,0,0,
		LoadPtr4,
		Halt
	};

	SilentMemory* memory = createSilentMemory(8000,8000);
	SilentGC* gc = createSilentGC(memory);
	SilentVM* vm = createSilentVM(memory,program,gc);

	silentVMStart(vm);

	printf("stack %i\n",*((char*)(memory->stack+8)));
	printf("stack ptr %i\n",memory->stackPointer);
	printf("stack T %i\n",memory->stackTypes->data[1]);

	deleteSilentGC(gc);
	deleteSilentMemory(memory);
	deleteSilentVM(vm);
	

    return 0;
}