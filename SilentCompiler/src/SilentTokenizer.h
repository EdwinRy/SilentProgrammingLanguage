#ifndef SILENT_TOKENIZER
#define SILENT_TOKENIZER
typedef enum silentTokenType
{
	silentUnrecognisedToken,
	//Symbols
	silentPlusToken,
	silentMinusToken,
	silentMultiplyToken,
	silentDivideToken,
	silentParenthesToken,
	silentCurlyBracketToken,
	silentBracketToken,
	silentQuotationToken,
	silentSemicolonToken,
	silentAssignToken,

	//Structure
	silentClassToken,
	silentFunctionToken,
	silentStructToken,
	silentArrayToken,
    silentVariableToken,
	
	//Access
	silentPublicToken,
	silentPrivateToken,
	silentProtectedToken,

	//Data
	silentNullToken,
	silentVoidToken,
	silentStringToken,
	silentIntegerToken,
	silentFloatToken,
	silentLongToken,
	silentDoubleToken,
	silentIdentifierToken
}silentTokenType;

//Token structure
typedef struct silentToken
{
    //Type of the token
	silentTokenType type;
    //Value of the token
	char* value;
}silentToken;

silentToken* silentTokenize(char* source, int* count);
#endif