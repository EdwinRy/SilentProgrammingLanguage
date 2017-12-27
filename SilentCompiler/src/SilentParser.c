#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
//Parse a function
silentFunction* silentParseFunction(silentToken* tokens, int* index)
{
	//Set up the function
	silentFunction* function = malloc(sizeof(silentFunction));
	*index+=1;
	
	//Get return type of the function
	if(!(
		(tokens[*index].type == silentIntegerToken) ||
		(tokens[*index].type == silentStringToken) ||
		(tokens[*index].type == silentFloatToken) ||
		(tokens[*index].type == silentVoidToken)
	))
	{
		//To do: Implement custom type return type
		printf("Invalid return type %s\n",tokens[*index].value);
		exit(1);
	}
	*index+=1;
	function->returnType = tokens[*index].type;

	//Get function name
	if(tokens[*index].type != silentIdentifierToken)
	{
		printf("Expected funtion name\n");
		exit(1);
	}
	function->name = tokens[*index].value;
	*index+=1;



	//Get function parameters
	if(tokens[*index].type != silentParenthesToken)
	{
		printf("Expected parentheses for function %s\n",function->name);
	}
	*index+=1;

	printf("declared function of name %s\n",function->name);

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

	//Parse function scope
	while(1)
	{
		switch(tokens[*index].type)
		{
			case silentClosingCurlyBracketToken:
				return function;
			break;
		}
		*index+=1;
	}
}

//Parse program
silentProgram* silentParseProgram(silentToken* tokens, int tokenCount)
{
	//Iterate through global scope tokens
	for(int i = 0; i < tokenCount; i++)
	{
		switch(tokens[i].type)
		{
			//If the token states a function declaration
			case silentFunctionToken:
				silentParseFunction(tokens,&i);
			break;

			//If the token is not supposed to be in the global scope
			default:
				printf("Invalid token in the global scope\n");
				printf("Invalid token: %s\n",tokens[i].value);
			break;
		}
	}
}