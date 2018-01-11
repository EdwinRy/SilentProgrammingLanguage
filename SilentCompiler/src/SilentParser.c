#include "SilentParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Reference to global structures
vector* globalVariables;
vector* structures;
vector* functions;

//Cast from token type to node type
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

//Check for existing types
char checkExistingType(silentToken token, int* size)
{
	//Test for built-in types
	switch(token.type)
	{
		case silentIntegerToken:
		case silentFloatToken:
			*size = 4;
			return 1;
		break;
		case silentLongToken:
		case silentDoubleToken:
			*size = 8;
			return 1;
		break;
		case silentStringToken:
			*size = 8;
			return 1;
		break;
	}
	//Test for structure types
	for(int i = 0; i < globalVariables->dataCount; i++)
	{
		if(strcmp(token.value, ((silentStruct*)vectorGet(structures,i))->name)==0)
		{
			*size = ((silentStruct*)vectorGet(structures,i))->size;
			return 2;
		}
	}
	return 0;
}

char checkExistingGlobal(char* value)
{
	for(int i = 0; i < globalVariables->dataCount; i++)
	{
		if(strcmp(
			value, ((silentVariable*)vectorGet(globalVariables,i))->name
			)==0)		
		{
			return 1;
		}
	}
	return 0;
}

//Check whether a function in the global scope has been declared
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
	//Allocate space for the variable
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type of the variable
	*index += 1;
	int typeSize = 0;
	char validity = checkExistingType(tokens[*index],&typeSize);
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		exit(1);
	}
	else
	{
		variable->value.size = typeSize;
		if(validity == 2){variable->value.type = silentStructType;}
		else{variable->value.type = castTokenToValueType(tokens[*index].type);}
	}
	
	//Parse name
	*index += 1;
	if(checkExistingGlobal(tokens[*index].value))
	{
		printf("Variable %s already exists!\n",tokens[*index].value);
		exit(1);
	}
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
	//printf("Declared global variable of name: %s\n",variable->name);
	//printf("Size of the variable: %i\n", variable->value.size);
	return variable;
}

silentVariable* silentParseStructVariable(
	silentToken* tokens, int* index, silentStruct* structure)
{
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type
	*index += 1;
	int typeSize = 0;
	char validity = checkExistingType(tokens[*index],&typeSize);	
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		exit(1);
	}
	else
	{
		variable->value.size = typeSize;
		if(validity == 2){variable->value.type = silentStructType;}
		else{variable->value.type = castTokenToValueType(tokens[*index].type);}
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
	structure->size = 0;
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
			silentVariable* variable = 
				silentParseStructVariable(tokens, index, structure);
			vectorPushBack(
				structure->variables,
				variable
			);
			structure->size += variable->value.size;
			free(variable);
		}
	}
	vectorPushBack(structures,structure);
	//printf("Declared structure of name: %s size: %i\n",structure->name,structure->size);
	//printf("Number of variables in the struct: %i\n",structure->variables->dataCount);
	return structure;
}

silentVariable* silentParseFunctionVariable(
	silentToken* tokens, int* index, silentFunction* function)
{
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type
	*index += 1;
	int typeSize = 0;
	char validity = checkExistingType(tokens[*index],&typeSize);
	
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		exit(1);
	}
	else
	{
		variable->value.size = typeSize;
		if(validity == 2){variable->value.type = silentStructType;}
		else{variable->value.type = castTokenToValueType(tokens[*index].type);}
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

silentVariable* silentParseParameterVariable(silentToken* tokens, int* index)
{
	silentVariable* variable = malloc(sizeof(silentVariable));
	//Parse type
	//*index += 1;
	int typeSize = 0;
	char validity = checkExistingType(tokens[*index],&typeSize);	
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		exit(1);
	}
	else
	{
		variable->value.size = typeSize;
		if(validity == 2){variable->value.type = silentStructType;}
		else{variable->value.type = castTokenToValueType(tokens[*index].type);}
	}
	//Parse name
	*index += 1;
	if(tokens[*index].type == silentIdentifierToken)
	{
		variable->name = tokens[*index].value;
	}
	*index += 1;
	if(tokens[*index].type != silentCommaToken &&
		tokens[*index].type != silentParenthesToken)
	{
		if(tokens[*index].type == silentAssignToken)
		{
			printf("Variable initialisation in a parameter not allowed: %s\n",
				variable->name);
			exit(1);
		}
		printf("Incorrect declaration of parameter: %s\n",variable->name);
		exit(1);
	}
	return variable;
}

vector* silentParseParameters(silentToken* tokens, int* index)
{
	vector* parameters = createVector(sizeof(silentVariable));
	//*index+=1;
	while(tokens[*index].type != silentOpenCurlyBracketToken)
	{
		vectorPushBack(parameters,silentParseParameterVariable(tokens, index));
		*index+=1;
	}
	*index+=1;
	return parameters;
}

silentExpression* silentParseExpression(
	silentToken* tokens, int* index,silentFunction* function)
{
	silentExpression* expression = malloc(sizeof(silentExpression));
	
	return expression;
}
//Parse a function
silentFunction* silentParseFunction(silentToken* tokens, int* index)
{
	//Set up the function
	silentFunction* function = malloc(sizeof(silentFunction));
	//function->parameters = createVector(sizeof(silentVariable*));
	*index+=1;
	int typeSize = 0;
	char validity = checkExistingType(tokens[*index],&typeSize);
	
	if(!validity)
	{
		printf("invalid type: %s\n",tokens[*index].value);
		exit(1);
	}
	else
	{
		function->returnType.size = typeSize;
		if(validity == 2){function->returnType.type = silentStructType;}
		else{function->returnType.type = castTokenToValueType(tokens[*index].type);}
	}

	//Get function name
	*index+=1;	
	if(tokens[*index].type != silentIdentifierToken)
	{
		printf("Expected function name\n");
		exit(1);
	}
	function->name = tokens[*index].value;
	*index+=1;

	//Get function parameters
	if(tokens[*index].type != silentParenthesToken)
	{
		printf("Expected parentheses for declaration of function %s\n",
			function->name);
	}
	*index+=1;

	silentVariable buffer[255];
	//Parse parameters
	if(tokens[*index].type != silentParenthesToken)
	{
		function->parameters = silentParseParameters(tokens,index);
	}
	else
	{
		function->parameters = createVector(sizeof(silentVariable));
		function->parameters->dataCount = 0;
	}
	
	printf("declared function of name: %s\n",function->name);

	//Parse function scope
	while(1)
	{
		switch(tokens[*index].type)
		{
			case silentClosingCurlyBracketToken:
				vectorPushBack(functions,function);
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