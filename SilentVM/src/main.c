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
	
	char factorial[] = 
	{
		Goto, 172,0,0,0,0,0,0,0, 		//+9	(0)			
		//End 0
		Push, 5, 0,0,0,0,				//+6	(9)
		Store,	0,0,0,0,0,0,0,0,		//+9	(15)
		Return,							//+1	(24)
		//End 1
		Push, 5, 1,0,0,0,				//+6	(25)
		Store,	0,0,0,0,0,0,0,0,		//+9	(31)
		Return,							//+1	(40)
		//Fib
		Load, 5, 0,0,0,0,0,0,0,0,		//+10	(41)
		Push, 5, 0,0,0,0,				//+6	(51)
		Equal,							//+1	(57)
		If, 9,0,0,0,0,0,0,0,			//+9	(58)
		Load, 5, 0,0,0,0,0,0,0,0,		//+10	(67)
		Push, 5, 1,0,0,0,				//+6	(77)
		Equal,							//+1	(83)
		If, 25,0,0,0,0,0,0,0,			//+9	(84)

		Load, 5, 0,0,0,0,0,0,0,0,		//+10	(93)		
		Push, 5, 1,0,0,0,				//+6	(103)		
		Sub,							//+1	(109)				
		Call, 41,0,0,0,0,0,0,0, 4,0,0,0,0,0,0,0,//+17	(110)
		Load, 5, 0,0,0,0,0,0,0,0,				//+10	(127)
		Push, 5, 2,0,0,0,						//+6	(137)	
		Sub,									//+1	(143)
		Call, 41,0,0,0,0,0,0,0, 4,0,0,0,0,0,0,0,//+17	(144)
		Add,									//+1	(161)
		Store, 0,0,0,0,0,0,0,0,					//+9	(162)
		Return,									//+1	(171)	
		//Main
		Push, 5, 3,0,0,0,						//+6	(172)	
		Call, 41,0,0,0,0,0,0,0, 4,0,0,0,0,0,0,0,//+17	(178)
		Halt									//+1	(195)
	};

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
	};

	SilentMemory* memory = createSilentMemory(8000,8000);
	SilentGC* gc = createSilentGC(memory);
	SilentVM* vm = createSilentVM(memory,factorial,gc);

	silentVMStart(vm);

	printf("stack %i\n",*((char*)(memory->stack)));
	printf("stack Type %i\n",memory->stackTypes->data[0]);
	printf("stack Type count %i\n",memory->stackTypes->ptr);

	deleteSilentGC(gc);
	deleteSilentMemory(memory);
	//deleteSilentVM(vm);
	

    return 0;
}