#include <stdlib.h>
#include <stdio.h> 
#include "SilentVM.h"

int main()
{
	int p = 0;
	char* b = malloc(50);
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	b[p++] = 0;
	SilentMemory* m = createSilentMemory(500,500);
	SilentThread* t = createSilentThread(m, b);
	printf("Hey Vsauce, Michael here!\n");

return 0;
}
