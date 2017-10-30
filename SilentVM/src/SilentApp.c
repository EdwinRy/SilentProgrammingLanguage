#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SilentVM.h"
int main(int argc, char** argv)
{
	if(argc == 1)
	{
		printf("usage: SilentVM <filename>\n");
	}
	else
	{
		if(argc == 2 && strcmp(argv[1],"-h")!=0)
		{
			char* bytecode;
			FILE *f;
			if((f = fopen(argv[1],"r"))==NULL)
			{
				printf("File %s doesn't exist!\n",argv[1]);
				exit(-1);
			}
			fseek(f,0,SEEK_END);
			bytecode = malloc(ftell(f));
			fseek(f,0,SEEK_SET);
			long count = 0;
			char c;
			while((c = fgetc(f))!=EOF)
			{
				bytecode[count++] = (char)c;
			}
			fclose(f);
			
			SilentMemory* m = createSilentMemory(count,count);
			SilentThread* t = createSilentThread(m,bytecode);
			executeSilentThread(t);
		}
	}
	return 0;
}
