#ifndef SILENT_PARSER
#define SILENT_PARSER
#include "SilentTokenizer.h"
#include "SilentHelper.h"
//Type of a node
/*
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
*/

typedef enum silentValueType
{
	silentFloat,
	silentInteger,
	silentIdentifier,
	silentStructType,
	silentNull,
	silentString
}silentValueType;

typedef enum silentExpressionParameterType
{
	silentExpressionValue,
	silentExpressionResult,
	silentExpressionVariable
}silentExpressionParameterType;

typedef enum silentExpressionType
{
	silentAssignment,
	silentAddition,
	silentSubtraction,
	silentMultiplication,
	silentDivision
}silentExpressionType;

//Node for a value
typedef struct silentValue
{
	//Type of the node
	silentValueType type;
	//Value of the node
	char* value;
}silentValue;

typedef struct silentVariable
{	
	silentValue value;
	char* name;
}silentVariable;

typedef struct silentExpression silentExpression;

typedef struct silentExpressionParameter
{
	silentExpressionType type;
	union
	{
		silentVariable* variable;
		silentValue* value;
		silentExpression* expression;
	};
}silentExpressionParameter;

//Node for an expression
struct silentExpression
{
	//Type of the node
	silentExpressionType type;
	//Expression parameters
	//E.G. 2 + 2; 
	//type: addition; parameters: 2, 2
	silentExpressionParameter parameters[2];
	//struct silentExpression* expressionParameters;
	//silentValue parameters[2];
};

typedef struct silentStruct
{
	vector* variables;
	char* name;
}silentStruct;

//Node for a function
typedef struct silentFunction
{
	//Name of the function
	char* name;
	//Return type
	silentValueType returnType;
	//Variables
	vector* variables;
	//Expressions
	vector* expressions;
	//Parameters
	vector* parameters;
}silentFunction;

//Node for the program
typedef struct silentProgram
{
	//Function array
	vector* functions;
	//Structures
	vector* structures;
	//Global variables
	vector* variables;
}silentProgram;


silentProgram* silentParseProgram(silentToken* tokens, int tokenCount);
#endif