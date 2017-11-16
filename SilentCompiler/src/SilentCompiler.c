#include "SilentCompiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

//Compiler structures
//Type of a token
typedef enum silentTokenType
{
	//Symbols
	silentPlusToken,
	silentMinusToken,
	silentMultiplyToken,
	silentDivideToken,
	silentParenthesToken,
	silentQuotationToken,
	silentSemicolonToken,

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
	silentIntegerToken,
	silentStringToken,
}silentTokenType;

//Token structure
typedef struct silentToken
{
    //Type of the token
	silentTokenType type;
    //Value of the token
	char* value;

}silentToken;

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


typedef struct silentValueNode
{
	silentNodeType type;
	char* value;
}silentValueNode;

typedef struct silentExpressionNode
{
	silentNodeType type;
	silentValueNode parameters[2];

}silentExpressionNode;

typedef struct silentFunctionNode
{
	silentExpressionNode* expressions;
}silentFunctionNode;

//Helper functions
//Test whether the passed token is a character
char silentTestLetter(char character)
{
	char* letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for(int i = 0; i < strlen(letters); i++)
	{
		if(character == letters[i])
		{
			return 1;
			break;
		}
	}
	return 0;
}

//Test whether the passed token is a number
char silentTestNumber(char character)
{
	char* numbers = "1234567890";
	for(int i = 0; i < strlen(numbers); i++)
	{
		if(character == numbers[i])
		{
			return 1;
			break;
		}
	}
	return 0;
}

//Test whether the passed token is whitespace
char silentTestWhitespace(char character)
{
	if(isspace(character))
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

//Tokenizer

silentToken* silentTokenize(char* source)
{
	long currentChar = 0;
	long currentLine = 0;
    char buffer[255];
	silentToken* tokens = malloc(10000);

	for(int i = 0; i < strlen(source); i++)
	{
		silentToken token;
	
		//Test for parentheses
		if((source[i] == *"(") || (source[i] == *")"))
		{
			token.type = silentParenthesToken;
			token.value = &source[i];
		}

		//Test for variable names
		else if(silentTestLetter(source[i]))
		{
			unsigned char count = 0;
			char* value;
			while(!silentTestWhitespace(source[i+count]))
			{
				buffer[count++] = source[i+count];
			}
			value = malloc(count);
			memcpy(value,buffer,count);
		}

		//Test for numbers
		else if(silentTestNumber(source[i]))
		{
			unsigned char count = 0;
			while(!silentTestWhitespace(source[i+count]))
			{
				buffer[count++] = source[i+count];
			}

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

//Parser


//Transform


//Code Generation




char* silentCompile(char* path, char* output)
{

	//char* rawSource = readAllText(path);//no AST
	char* source = "func int main(){var int testVariable = 5;return 0;}";
	silentToken* tokens = silentTokenize(source);

}

//Testing
int main()
{
	char* source = "test.silent";
	char* out = "out";
	silentCompile(source,out);
	return 0;
}
