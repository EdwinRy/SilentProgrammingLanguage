#include "SilentVM.h"
#include "SilentLib.h"
#include <stdio.h>
#include <time.h>
char* readAllText(char* path)
{
    char* text;
	FILE *f;
	if((f = fopen(path,"rb"))==NULL)
	{
		printf("File %s doesn't exist!\n",path);
		exit(-1);
	}
	fseek(f,0,SEEK_END);
	text = malloc(ftell(f)+1);
	printf("file size: %lu\n", ftell(f));
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

	//SilentMemory* memory = createSilentMemory(80000,80000);
	//SilentGC* gc = createSilentGC(memory);
	//SilentVM* vm = createSilentVM(memory,program,gc);


	clock_t start = clock();
	SilentStartVM(program);
	clock_t end = clock();

	double timeSpent = (double)(end-start) / CLOCKS_PER_SEC;

	// printf("%i\n", vm->memory->stackPointer);
	//unsigned long long t;
	//memcpy(&t, vm->memory->stack, 8);
	//printf("%u\n", t);
	printf("Time taken: %f seconds\n", timeSpent);

	//deleteSilentGC(gc);
	//deleteSilentMemory(memory);
	//deleteSilentVM(vm);
	

    return 0;
}