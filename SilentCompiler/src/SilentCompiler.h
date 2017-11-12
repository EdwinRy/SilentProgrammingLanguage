#include <stdlib.h>
#include <string>
enum silentTokenType
{
	//Symbols
	silentPlus,
	silentMinus,
	silentMultiply,
	silentDivide,
	silentParenthes,

	//Structure
	silentClass,
	silentFunction,
	silentStruct,
	silentArray,

	
	//Access
	silentPublic,
	silentPrivate,
	silentProtected,

	//Data
	silentByte,
	silentInteger,
	silentFloat,
	silentLong,
	silentDouble,
	silentString,
};

typedef struct silentToken
{
	silentTokenType type;
	std::string value;

}silentToken;

std::string* silentCompile(std::string path);
