#include "Lexer.h"

void Tokenize(char* source, char* tokens)
{
	char* CodeBuffer = malloc(50);
	unsigned int CodeBufferPointer = 0;



}

char isEqualString(char* string1, char* string2) 
{
	int len1 = strlen(string1);
	int len2 = strlen(string2);
	if (len1 == len2) 
	{
		for(int i = 0; i <= len1; i++)
		{
			if (string1[i] != string2[i]) {return 0;}
		}
		return 1;
	}
	else {return 0;}
}

