#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vector* globalVariables;
vector* structures;
vector* functions;

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

int checkExistingType(silentToken token)
{

	switch(token.type)
	{
		case silentIntegerToken:
		case silentFloatToken:
			return 4;
		break;
		case silentLongToken:
		case silentDoubleToken:
			return 8;
		break;
		case silentStringToken:
			return 1;
		break;
	}
	for(int i = 0; i < globalVariables->dataCount; i++)
	{
		if(strcmp(token.value, ((silentStruct*)vectorGet(structures,i))->name)==0)
		{
			return ((silentStruct*)vectorGet(structures,i))->size;
		}
	}
	return 0;
}

char checkExistingFunction(silentToken token)
{
	for(int i = 0; i < functions->dataCount; i++)
	{
		if(strcmp(((silentFunction*)vectorGet(functions,i))->name,token.value)==0)
		{
			return 1;
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
	if(validity > 0)
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
	}
	else if(validity == 3)
	{
		variable->value.type = silentStructType;
	}
	
/*
	if((tokens[*index].type == silentIntegerToken)&&
		(tokens[*index].type == silentFloatToken))
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
		variable->value.size = 4;
	}

	else if((tokens[*index].type == silentLongToken)&&
		(tokens[*index].type == silentDoubleToken))
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
		variable->value.size = 8;
	}

	else if((tokens[*index].type == silentStringToken))
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
		variable->value.size = 0;
	}
*/
	//Parse name
	*index += 1;
	if(tokens[*index].type == silentIdentifierToken)
	{
		variable->name = tokens[*index].value;
	}
	*index += 1;
	if(tokens[*index].type != silentSemicolonToken)
	{
		if(tokens[*index].type == silentAssignToken)
		{
			printf("Variable initialisation in global scope not allowed: %s\n",
				variable->name);
			exit(1);
		}
		printf("Incorrect declaration of global variable: %s\n",variable->name);
		exit(1);
	}
	vectorPushBack(globalVariables,variable);
	printf("Declared global variable of name: %s\n",variable->name);
	return variable;
}

silentVariable* silentParseStructVariable(
	silentToken* tokens, int* index, silentStruct* structure)
{
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type
	*index += 1;
	char validity = checkExistingType(tokens[*index]);
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		printf("for variable inside struct: %s\n",structure->name);
		exit(1);
	}
	if(validity == 1)
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
	}
	else if(validity == 2)
	{
		if(strcmp(tokens[*index].value,structure->name)==0)
		{
			printf("variable type can't be the same as structure name\n");
			printf("structure with invalid item: %s\n",structure->name);
			exit(1);
		}
		variable->value.type = silentStructType;
	}
	//Parse name
	*index += 1;
	if(tokens[*index].type == silentIdentifierToken)
	{
		variable->name = tokens[*index].value;
	}
	*index += 1;
	if(tokens[*index].type != silentSemicolonToken)
	{
		if(tokens[*index].type == silentAssignToken)
		{
			printf("Variable initialisation in a structure not allowed: %s\n",
				variable->name);
			exit(1);
		}
		printf("Incorrect declaration of struct variable: %s\n",variable->name);
		exit(1);
	}
	//printf("Declared %s struct variable: %s\n", structure->name, variable->name);
	return variable;
}

//Parse structure
silentStruct* silentParseStructure(silentToken* tokens, int* index)
{
	silentStruct* structure = malloc(sizeof(silentStruct));
	structure->variables = createVector(sizeof(silentVariable));
	//Parse name
	*index+=1;
	if(tokens[*index].type != silentIdentifierToken)
	{
		printf("Expected struct name\n");
		exit(1);
	}
	structure->name = tokens[*index].value;

	//Parse variables
	*index+=1;
	if(tokens[*index].type != silentOpenCurlyBracketToken)
	{
		printf("Expected scope for struct: %s\n", structure->name);
		exit(1);
	}

	*index+=1;
	for(;tokens[*index].type != silentClosingCurlyBracketToken;*index+=1)
	{
		if(tokens[*index].type == silentVariableToken)
		{
			vectorPushBack(
				structure->variables,
				silentParseStructVariable(tokens, index, structure)
			);
		}
	}
	vectorPushBack(structures,structure);
	printf("Declared structure of name: %s\n",structure->name);
	printf("Number of variables in the struct: %i\n",structure->variables->dataCount);
	return structure;
}

silentVariable* silentParseFunctionVariable(
	silentToken* tokens, int* index, silentFunction* function)
{
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type
	*index += 1;
	char validity = checkExistingType(tokens[*index]);
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		printf("for variable inside function: %s\n",function->name);
		exit(1);
	}
	if(validity >= 1)
	{
		variable->value.type = castTokenToValueType(tokens[*index].type);
	}
	//Parse name
	*index += 1;
	if(tokens[*index].type == silentIdentifierToken)
	{
		variable->name = tokens[*index].value;
	}
	*index += 1;
	if(tokens[*index].type != silentSemicolonToken)
	{
		if(tokens[*index].type == silentAssignToken)
		{
			*index -= 1;
			return variable;
		}
		printf("Incorrect declaration of variable: %s\n",variable->name);
		exit(1);
	}
	//printf("Declared %s struct variable: %s\n", structure->name, variable->name);
	return variable;
}

silentExpression* silentParseExpression(
	silentToken* tokens, int* index,silentFunction* function)
{
	silentExpression* expression = malloc(sizeof(silentExpression));
	if(tokens[*index].type == silentIdentifierToken)
	{		
		if(checkExistingFunction(tokens[*index]))
		{

		}
		else
		{
			*index += 1;

		}
	}
	else
	{
		printf("An expression needs to start with an identifier\n");
		printf("funcion:%s\n",function->name);
		exit(1);
	}
	return expression;
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
	vectorPushBack(functions,function);
	return function;
}

//Parse program
silentProgram* silentParseProgram(silentToken* tokens, int tokenCount)
{
	//Setup global variables
	globalVariables = createVector(sizeof(silentVariable));
	structures = createVector(sizeof(silentStruct));
	functions = createVector(sizeof(silentFunction));


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
	return program;
}