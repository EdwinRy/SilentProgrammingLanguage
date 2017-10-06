#include <stdlib.h>
#include <stdio.h> 
#include "SilentVM.h"

int main()
{
	int p = 0;
	char* b = malloc(500);
	b[p++] = Push1;
	b[p++] = 10;
	b[p++] = 0;
	b[p++] = Push1;
	b[p++] = 254;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	SilentMemory* m = createSilentMemory(500,500);
	SilentThread* t = createSilentThread(m, b);

	executeSilentThread(t);
	printf("%i\n", m->stack[1]);

return 0;
}
