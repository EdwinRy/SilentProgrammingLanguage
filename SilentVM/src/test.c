#include <stdlib.h>
#include <stdio.h> 
#include "SilentVM.h"

int main()
{
	int p = 0;
	char* b = malloc(500);
	b[p++] = PushX;
	b[p++] = 4;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;//
	b[p++] = 255;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	//b[p++] = PopX;
	//b[p++] = 4;
	//b[p++] = 0;
	//b[p++] = 0;
	//b[p++] = 0;
	//b[p++] = 0;
	//b[p++] = 0;
	//b[p++] = 0;
	//b[p++] = 0;
	b[p++] = Push4;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 1;
	b[p++] = Halt;
	SilentMemory* m = createSilentMemory(500,500);
	SilentThread* t = createSilentThread(m, b);

	executeSilentThread(t);
	//printf("%i",m->stackPointer);
	printf("%i\n", *(int*)(m->stack + 0));

return 0;
}
