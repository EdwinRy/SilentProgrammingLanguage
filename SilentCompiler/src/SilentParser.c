#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
//Parse a function
silentFunction* silentParseFunction(silentToken* tokens, int* index)
{
	//Set up the function
	silentFunction function;
	*index++;
	
	if(!(
		(tokens[*index].type == silentIntegerToken) ||
		(tokens[*index].type == silentStringToken) ||
		(tokens[*index].type == silentFloatToken) ||
		(tokens[*index].type == silentVoidToken)
	))
	{
		//Implement custom type return type
		printf("Invalid return type\n");
		exit(0);
	}

	*index++;

	if(tokens[*index].type != silentIdentifierToken)
	{
		printf("Expected funtion name\n");
		exit(0);
	}

	function.name = tokens[*index].value;

	*index++;

	if(tokens[*index].type != silentParenthesToken)
	{
		printf("Expected parentheses for function %s\n",function.name);
	}

	while(1)
	{
		switch(tokens[*index].type)
		{
			
		}
		*index++;
	}
}

silentProgram* silentParseProgram(silentToken* tokens, int tokenCount)
{
	for(int i = 0; i < tokenCount; i++)
	{
		switch(tokens[i].type)
		{
			case silentFunctionNode:
				silentParseFunction(tokens,&i);
			break;

			default:
				printf("Invalid token in the global scope\n");
			break;
		}
	}
}