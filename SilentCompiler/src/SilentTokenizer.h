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
	silentOpenCurlyBracketToken,
	silentClosingCurlyBracketToken,
	silentBracketToken,
	silentQuotationToken,
	silentSemicolonToken,
	silentAssignToken,
	silentCommaToken,
	silentFullstopToken,

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
	silentTextToken,
	silentIntegerToken,
	silentNumberToken,
	silentFloatToken,
	silentDecimalToken,
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

//Turn source code into tokens array
silentToken* silentTokenize(char* source, int* count);
#endif