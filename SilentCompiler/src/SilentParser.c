#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vector* globalVariables;
vector* structures;

silentValueType castTokenToValueType(silentTokenType type)
{
	switch(type)
	{
		case silentFloatToken:
			return silentFloat;
		break;
		case silentIntegerToken:
			return silentInteger;
		break;
		case silentIdentifierToken:
			return silentIdentifier;
		break;
		case silentStructToken:
			return silentStructType;
		break;
		case silentNullToken:
			return silentNull;
		break;
		case silentStringToken:
			return silentString;
		break;
	}
}

char checkExistingType(silentToken token)
{

	switch(token.type)
	{
		case silentIntegerToken:
			return 1;
		break;
		case silentFloatToken:
			return 1;
		break;
		case silentLongToken:
			return 1;
		break;
		case silentDoubleToken:
			return 1;
		break;
		case silentStringToken:
			return 1;
		break;
	}
	for(int i = 0; i < globalVariables->dataCount; i++)
	{
		if(strcmp(token.value, (char*)vectorGet(structures,i))==0)
		{
			return 2;
		}
	}
	return 0;
}

//Parse global variable
silentVariable* silentParseGlobalVariable(silentToken* tokens, int* index)
{
	//printf("%s\n",tokens[*index].value);
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type
	*index += 1;
	char validity = checkExistingType(tokens[*index]);
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		exit(1);
	}
	if(validity == 1)
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
	}
	else if(validity == 2)
	{
		variable->value.type = silentStructType;
	}
	//Parse name
	*index += 1;
	if(tokens[*index].type == silentIdentifierToken)
	{
		variable->name = tokens[*index].value;
	}
	*index += 1;
	vectorPushBack(globalVariables,variable);
	printf("Declared global variable of name: %s\n",variable->name);
	return variable;
}

//Parse structure
silentStruct* silentParseStructure(silentToken* tokens, int* index)
{

}
//Parse a function
silentFunction* silentParseFunction(silentToken* tokens, int* index)
{
	//Set up the function
	silentFunction* function = malloc(sizeof(silentFunction));
	*index+=1;
	
	char validity = checkExistingType(tokens[*index]);
	//Get return type of the function
	if(!validity)
	{
		//To do: Implement custom type return type
		printf("Invalid return type %s\n",tokens[*index].value);
		exit(1);
	}
	if(validity == 1)
	{
		function->returnType = castTokenToValueType(tokens[*index].type);
	}
	else if(validity == 2)
	{
		function->returnType = silentStructType;
	}

	//Get function name
	*index+=1;
	
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
		printf("Expected parentheses for decleration of function %s\n",function->name);
	}
	*index+=1;

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
	printf("declared function of name: %s\n",function->name);

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
	return function;
}

//Parse program
silentProgram* silentParseProgram(silentToken* tokens, int tokenCount)
{
	//Setup global variables
	globalVariables = createVector(sizeof(silentVariable));
	structures = createVector(sizeof(silentStruct));


	//Create program
	silentProgram* program = malloc(sizeof(silentProgram));
	program->functions = createVector(sizeof(silentFunction));
	program->structures = createVector(sizeof(silentStruct));
	program->variables = createVector(sizeof(silentVariable));

	//Iterate through global scope tokens
	for(int i = 0; i < tokenCount; i++)
	{
		switch(tokens[i].type)
		{
			//If the token states a function declaration
			case silentFunctionToken:
				vectorPushBack(program->functions,silentParseFunction(tokens,&i));
			break;

			//If the token declares structure
			case silentStructToken:		
				vectorPushBack(program->structures,silentParseStructure(tokens,&i));
			break;

			//If the token declares global variable
			case silentVariableToken:
				vectorPushBack(program->variables,silentParseGlobalVariable(tokens,&i));
			break;

			//If the token is not supposed to be in the global scope
			default:
				printf("Invalid token in the global scope\n");
				printf("Invalid token: %s\n",tokens[i].value);
				exit(1);
			break;
		}
		//printf("number of functions declared:%i\n",program->functions->dataCount);
	}
}