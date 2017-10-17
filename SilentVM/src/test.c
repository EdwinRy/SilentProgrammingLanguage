#include <stdlib.h>
#include <stdio.h> 
#include "SilentVM.h"

int main()
{
	int p = 0;
	char* b = malloc(500);
	//b[p++] = Alloc4;
	b[p++] = Push4;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 1;
	b[p++] = Push4;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 1;
	b[p++] = 0;
	b[p++] = Alloc4;
	b[p++] = Store4;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = Load4;//end
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = Pop4;
	b[p++] = Pop4;
	b[p++] = Load4;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = Halt;

	SilentMemory* m = createSilentMemory(500,500);
	SilentThread* t = createSilentThread(m, b);

	executeSilentThread(t);
	printf("%i\n",m->stackPointer);
	printf("%i\n", *(int*)(m->stack + 0));

return 0;
}
