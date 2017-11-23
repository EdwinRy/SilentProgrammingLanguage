#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
//Parse a function
silentFunction* silentParseFunction(silentToken* tokens, int* index)
{
	//Set up the function
	silentFunction function;
	*index++;
	
	//Get return type
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

	//Get function name
	if(tokens[*index].type != silentIdentifierToken)
	{
		printf("Expected funtion name\n");
		exit(0);
	}
	function.name = tokens[*index].value;
	*index++;

	//Get function parameters
	if(tokens[*index].value != silentParenthesToken)
	{
		printf("Expected parentheses for function %s\n",function.name);
	}
	*index++;

	silentVariable buffer[255];
	//Parse parameters
	int parameterCount = 0;
	while(tokens[*index].type != silentParenthesToken)
	{
		if(tokens[*index].type != silentCommaToken)
		{
			
			parameterCount++;	
		}
		*index++;
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