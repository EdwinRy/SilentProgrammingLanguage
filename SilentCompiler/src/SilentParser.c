#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
//Parse a function
silentFunction* silentParseFunction(silentToken* tokens, int* index)
{
	//Set up the function
	silentFunction function;
	*index+=1;
	
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
	*index+=1;

	function.returnType = tokens[*index].type;

	//Get function name
	if(tokens[*index].type != silentIdentifierToken)
	{
		printf("Expected funtion name\n");
		exit(0);
	}
	function.name = tokens[*index].value;
	*index+=1;

	//Get function parameters
	if(tokens[*index].type != silentParenthesToken)
	{
		printf("Expected parentheses for function %s\n",function.name);
	}
	*index+=1;

	printf("declared function of name %s",function.name);

	silentVariable buffer[255];
	//Parse parameters
	int parameterCount = 0;
	while(tokens[*index].type != silentParenthesToken)
	{
		if(tokens[*index].type != silentCommaToken)
		{
			
			parameterCount+=1;	
		}
		*index+=1;
	}


	while(1)
	{
		switch(tokens[*index].type)
		{
			case silentClosingCurlyBracketToken:
				return &function;
			break;
		}
		*index+=1;
	}
}

silentProgram* silentParseProgram(silentToken* tokens, int tokenCount)
{
	printf("started parsing\n");
	for(int i = 0; i < tokenCount; i++)
	{
		switch(tokens[i].type)
		{
			case silentFunctionNode:
				printf("parsing function\n");
				silentParseFunction(tokens,&i);
				printf("done parsing function\n");
			break;

			default:
				printf("Invalid token in the global scope\n");
				printf("Invalid token: %s\n",tokens[i].value);
			break;
		}
	}
}