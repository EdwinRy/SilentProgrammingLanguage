#include "SilentTokenizer.h"
#include "SilentHelper.h"

#ifndef SILENT_PARSER
#define SILENT_PARSER

//Type of the value stored by the variable
typedef enum silentValueType
{
	silentFloat,
	silentInteger,
	silentDouble,
	silentLong,
	silentIdentifier,
	silentStructType,
	silentNull,
	silentString
}silentValueType;

//Type of a value in an expression
typedef enum silentExpressionParameterType
{
	silentExpressionValue,
	silentExpressionResult,
	silentExpressionVariable
}silentExpressionParameterType;

//Type of expression
typedef enum silentExpressionType
{
	silentAssignment,
	silentAddition,
	silentSubtraction,
	silentMultiplication,
	silentDivision
}silentExpressionType;

//Predeclare structure
typedef struct silentStruct silentStruct;

//Node for a value
typedef struct silentValue
{
	//Type of the node
	silentValueType type;
	//Value size
	int size;
	//Value of the node
	char* value;
}silentValue;

//Variable node
typedef struct silentVariable
{	
	silentValue value;
	char* name;
}silentVariable;

//Predeclare the expression node
typedef struct silentExpression silentExpression;

//A struct used to express an expression
typedef struct silentExpressionParameter
{
	silentExpressionType type;
	union
	{
		silentVariable* variable;
		silentValue* value;
		silentExpression* expression;
		vector* parameters;
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

//Node for a structure
typedef struct silentStruct
{
	vector* variables;
	char* name;
	//Size in bytes for the structure
	int size;
}silentStruct;

//Node for a function
typedef struct silentFunction
{
	//Name of the function
	char* name;
	//Return type
	silentValue returnType;
	//Variables
	vector* variables;
	//Expressions
	vector* expressions;
	//Parameters
	vector* parameters;
}silentFunction;

//Node for a function call
typedef struct silentFunctionCall
{

	silentFunction* function;
	vector* parameters;
}silentFunctionCall;

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

//Turn token array into a syntax tree
silentProgram* silentParseProgram(silentToken* tokens, int tokenCount);
#endif