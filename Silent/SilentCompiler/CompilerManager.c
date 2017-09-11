#include "Compiler.h"
#include <string.h>

typedef struct fileBuffer 
{
	long long lastChar;
	int numberOfFiles;
	char* buffer;
}fileBuffer;

void appendFileToBuffer(fileBuffer* buffer, char* path) 
{
	FILE *source = fopen(path, "r");
	fseek(source, 0, SEEK_END);
	long long fileSize = ftell(source);
	fseek(source, 0, SEEK_SET);
	char* textBuffer = calloc(sizeof(char), fileSize + 1);
	fread(textBuffer, 1, fileSize, source);
	fclose(source);


	buffer->buffer = realloc(buffer->buffer, buffer->lastChar + fileSize);
	memcpy(buffer->buffer + buffer->lastChar, textBuffer, fileSize);
}

int main(int argc, char * argv[]) 
{
	for (int i = 0; i <= argc-1; i++) 
	{
		if (strcmp("-am", argv[i]) == 0) 
		{
			//allocate space for file buffer
			fileBuffer* buffer = malloc(sizeof(fileBuffer));
			buffer->lastChar = 0;
			buffer->numberOfFiles = atoi(argv[i + 1]);
			buffer->buffer = malloc(1);
			//check for incorrect syntax
			if (i != 1) { printf("assembly mode used incorrectly in this context"); return -1; }
			char* outputFile = NULL;

			for (int x = 0; x < argc; x++) 
			{
				if (strcmp(argv[x],"-o") == 0) 
				{
					outputFile = argv[x + 1];
					break;
				}
			}

			if(outputFile == NULL)
				outputFile = argv[3];
			

			//iterate through all input files
			for (int y = 0; y < atoi(argv[i + 1]); y++) 
			{
				//appendFileToBuffer(buffer,argv[y + 3]);
			}

			//allocate space for assembly object
			assembly* asm = malloc(sizeof(assembly));
			asm->code = buffer->buffer;
			asm->codeSize = buffer->lastChar;
			asm->outputFile = outputFile;
			//compile assembly file
			CompileAssembly(asm);
			
			break;
		}
	}
	getchar();
	return 0;
}