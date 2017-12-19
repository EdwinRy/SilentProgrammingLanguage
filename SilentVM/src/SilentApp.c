#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SilentVM.h"
int main(int argc, char** argv)
{
	if(argc == 2)
	{
		if(strcmp(argv[1],"-h")==0)
		{
			printf("SilentVM -h to display this message\n");
			printf("SilentVM <filename> to execute the file\n");
		}

		else
		{
			char* bytecode;
			FILE *f;
			if((f = fopen(argv[1],"rb"))==NULL)
			{
				printf("File %s doesn't exist!\n",argv[1]);
				exit(-1);
			}

			fseek(f,0,SEEK_END);
			long fileSize = ftell(f);
			rewind(f);
			bytecode = malloc(fileSize);
			fread(bytecode,fileSize,1,f);
			fclose(f);

			SilentMemory* mem = createSilentMemory(1000,1000);
			SilentThread* thread = createSilentThread(mem,bytecode);
			executeSilentThread(thread);
			printf("%i\n",*((int*)(mem->stack + 0)));
		}
	}

	else
	{
		printf("usage: SilentVM <filename>\n");
	}
	return 0;
}
