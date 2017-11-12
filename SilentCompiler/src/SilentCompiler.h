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
}

typedef struct silentToken
{
	silentTokenType type;
	string value;

}silentToken;
