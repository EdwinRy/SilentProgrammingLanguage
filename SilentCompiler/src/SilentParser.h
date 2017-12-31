#ifndef SILENT_PARSER
#define SILENT_PARSER
#include "SilentTokenizer.h"
#include "SilentHelper.h"
//Type of a node
typedef enum silentNodeType
{
	//Value
	silentStringNode,
	silentIntegerNode,
	silentFloatNode,

	//Functionality
	silentAdditionNode,
	silentSubtractionNode,
	silentMultiplicationNode,
	silentDivisionNode,
	silentAssignNode,
	silentReturnNode,

	//Structure
	silentIfNode,
	silentWhileNode,
	silentForNode,
	silentFunctionNode,
}silentNodeType;

//Node for a value
typedef struct silentValue
{
	//Type of the node
	silentNodeType type;
	//Value of the node
	char* value;
}silentValue;

typedef struct silentVariable
{	
	silentValue value;
	char* name;
}silentVariable;

//Node for an expression
typedef struct silentExpression
{
	//Type of the node
	silentNodeType type;
	//Expression parameters
	//E.G. 2 + 2; 
	//type: addition; parameters: 2, 2
	silentValue parameters[2];
}silentExpression;

//Node for a function
typedef struct silentFunction
{
	//Array of expressions
	silentExpression* expressions;
	//Name of the function
	char* name;
	//Return type
	silentTokenType returnType;
	//Parameters
	silentValue* parameters;
	//Number of parameters
	int parameterCount;
}silentFunction;

//Node for the program
typedef struct silentProgram
{
	//Number of functions
	int functionCount;
	//Function array
	//silentFunction* functions;
	vector* functions;
}silentProgram;


silentProgram* silentParseProgram(silentToken* tokens, int tokenCount);
#endif