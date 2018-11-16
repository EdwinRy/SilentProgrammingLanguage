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

	printf("%i\n", vm->memory->stackPointer);
	printf("%i\n", vm->memory->stack[0]);

	deleteSilentGC(gc);
	deleteSilentMemory(memory);
	//deleteSilentVM(vm);
	

    return 0;
}