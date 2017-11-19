#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//HELPER
//Read all characters in a file
char* readAllText(char* path)
{
    //Text buffer to be returned
    char* text;
    //File to be read from
	FILE *f;
    //If opening of the file fails
	if((f = fopen(path,"r"))==NULL)
	{
        //output about file not existing
		printf("File %s doesn't exist!\n",path);
        //Close the program
		exit(-1);
	}
    //Seek the end of the file
	fseek(f,0,SEEK_END);
    //Allocate enough space for the entire file
	text = malloc(ftell(f));
    //Rewind the file back to beginning
	fseek(f,0,SEEK_SET);
    //Declare counter for the character array
	long count = 0;
    //Initialise buffer for single character
	char c;
    //Iterate for as long as the character is not
    //the end of the file
	while((c = fgetc(f))!=EOF)
	{
        //Add the character
		text[count++] = (char)c;
	}
    //Close the file
    fclose(f);
    //Return the source
    return text;
}

//Helper functions
//Test whether the passed token is a character
char silentTestLetter(char character)
{
	//All characters to be recognised
	//as letters
	char* letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	//Iterate through the array of letters
	for(int i = 0; i < strlen(letters); i++)
	{
		//Check whether the character is in the array
		if(character == letters[i])
		{
			//Return true
			return 1;
			//Stop iteration (just to be sure)
			break;
		}
	}
	//Return false
	return 0;
}

//Test whether the passed token is a number
char silentTestNumber(char character)
{
	//Characters recognised as numbers
	char* numbers = "1234567890";
	//Iterate through array of numbers
	for(int i = 0; i < strlen(numbers); i++)
	{
		//If character is in the array
		if(character == numbers[i])
		{
			//Return true
			return 1;
			//break;
		}
	}
	//Return false
	return 0;
}

//Test whether the passed token is whitespace
char silentTestWhitespace(char character)
{
	//If the character is a whitespace
	if(isspace(character))
	{
		//Return true
		return 1;
	}

	//If the character is not whitespace
	else
	{
		//Return false
		return 0;
	}
}

//TOKENIZER
//Type of a token
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

//Tokenizer
//Extract the array of tokens from source
silentToken* silentTokenize(char* source)
{
	//Current character
	long currentChar = 0;
	//Current line
	long currentLine = 0;
	//Character buffer for parsing
    char buffer[255];
	//Array of tokens
	silentToken* tokens = malloc(strlen(source));

	//Iterate through the source
	for(int i = 0; i < strlen(source); i++)
	{
		//Create new token
		silentToken token;
		//Assign unrecognised token type by default
		token.type = silentUnrecognisedToken;
		//Assign the value to "NULL" by default
		token.value = "NULL";

		//Test for semicolon
		if(source[i] == *";")
		{
			token.type = silentSemicolonToken;
			token.value = ";";
		}

		//Test for assignment
		else if(source[i] == *"=")
		{
			token.type = silentAssignToken;
			token.value = "=";
		}
		//Test for parentheses
		else if((source[i] == *"(") || (source[i] == *")"))
		{
			//Assign the parentheses type
			token.type = silentParenthesToken;
			//Allocate 2 bytes for the string
			token.value = malloc(2);
			//Assign first character to the parentheses
			token.value[0] = source[i];
			//Null terminate the string
			token.value[1] = '\0';
		}

		//Test for curly brackets
		else if((source[i] == *"{") || (source[i] == *"}"))
		{
			//Assign the parentheses type
			token.type = silentCurlyBracketToken;
			//Allocate 2 bytes for the string
			token.value = malloc(2);
			//Assign first character to the parentheses
			token.value[0] = source[i];
			//Null terminate the string
			token.value[1] = '\0';
		}

		//Test for multicharacter tokens
		else if(silentTestLetter(source[i]))
		{
			//Number of characters
			unsigned char count = 0;
			//prepare space for the value
			char* value;
			//As long as the current character isn't whitespace
			while(silentTestLetter(source[i+count]) ||
				silentTestNumber(source[i+count]))
			{
				//Assign buffer to the current character
				buffer[count++] = source[i+count];
			}
			//Allocate space for the value and terminator
			value = malloc(count+1);
			//Copy the value from the buffer
			memcpy(value,buffer,count);
			value[count] = '\0';
			token.value = value;
			token.type = silentIdentifierToken;
			//Test for non-identifier tokens
			//If the token is a function
			if(strcmp(token.value, "func"))
			{
				token.type = silentFunctionToken;
			}
			else if(strcmp(token.value, "var"))
			{
				token.type = silentVariableToken;
			}
			else if(strcmp(token.value, "int"))
			{
				token.type = silentIntegerToken;
			}
			else if(strcmp(token.value, "float"))
			{
				token.type = silentFloatToken;
			}
			i += count-1;
		}

		//Test for numbers
		else if(silentTestNumber(source[i]))
		{
			unsigned char count = 0;
			char* value;
			char floatVal;
			while(silentTestNumber(source[i+count]));
			{
				//if(source[i+count] == *"."){floatVal = 1;}
				printf("true\n");
				buffer[count++] = source[i+count];
			}

			//Allocate space for the value and terminator
			value = malloc(count+1);
			//Copy the value from the buffer
			memcpy(value,buffer,count);
			value[count] = '\0';
			token.value = value;
			token.type = silentIntegerToken;
			if(floatVal){token.type = silentFloatToken;}

            i += count-1;
		}

		//Test for quotations
		else if(source[i] == *"\"")
		{
			token.type = silentQuotationToken;
			token.value = "";
			for(int j = 1; source[i+j] != *"\"";j++)
			{
				token.value += source[i+j];
			}
		}

		//Test for whitespaces
		else if(silentTestWhitespace(source[i]))
		{
			continue;
		}

		else{
			continue;
		}
		tokens[i] = token;
		printf("%s\n",token.value);
	}
	return tokens;
}

//PARSER
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
}silentNodeType;

//Node for a value
typedef struct silentValueNode
{
	//Type of the node
	silentNodeType type;
	//Value of the node
	char* value;
}silentValueNode;

//Node for an expression
typedef struct silentExpressionNode
{
	//Type of the node
	silentNodeType type;
	//Expression parameters
	//E.G. 2 + 2; 
	//type: addition; parameters: 2, 2
	silentValueNode parameters[2];
}silentExpressionNode;

//Node for a function
typedef struct silentFunctionNode
{
	//Array of expressions
	silentExpressionNode* expressions;
}silentFunctionNode;

char* silentCompile(char* path, char* output)
{

	char* rawSource = readAllText(path);//no AST
	//char* rawSource = "func int main(){var int testVariable = 5;return 0;}";
	silentToken* tokens = silentTokenize(rawSource);

}

//Testing
int main()
{
	char* source = "test.silent";
	char* out = "out";
	silentCompile(source,out);
	return 0;
}
