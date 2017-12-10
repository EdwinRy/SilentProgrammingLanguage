#include "SilentTokenizer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//Extract the array of tokens from source
silentToken* silentTokenize(char* source, int* tokenCount)
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
		if(source[i] == ';')
		{
			token.type = silentSemicolonToken;
			token.value = ";";
		}

		//Test for comma
		if(source[i] == ',')
		{
			token.type = silentCommaToken;
			token.value = ",";
		}

		//Test for assignment
		else if(source[i] == '=')
		{
			token.type = silentAssignToken;
			token.value = "=";
		}
		//Test for parentheses
		else if((source[i] == '(') || (source[i] == ')'))
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
		else if(source[i] == '{')
		{
			//Assign the parentheses type
			token.type = silentOpenCurlyBracketToken;
			//Allocate 2 bytes for the string
			token.value = malloc(2);
			//Assign first character to the parentheses
			token.value[0] = source[i];
			//Null terminate the string
			token.value[1] = '\0';
		}

		else if(source[i] == '}')
		{
			//Assign the parentheses type
			token.type = silentClosingCurlyBracketToken;
			//Allocate 2 bytes for the string
			token.value = malloc(2);
			//Assign first character to the parentheses
			token.value[0] = source[i];
			//Null terminate the string
			token.value[1] = '\0';
		}

		//Test for multicharacter tokens
		else if(isalpha(source[i]))
		{
			//Number of characters
			unsigned char count = 0;
			//prepare space for the value
			char* value;
			//As long as the current character isn't whitespace
			while(isalpha(source[i+count]) ||
				isdigit(source[i+count]))
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
		else if(isdigit(source[i]))
		{
			char* value;
			char floatVal = 0;
			char count = 0;
			while(isdigit(source[i]))
			{
				buffer[count] = source[i];
				i++;
				count++;
			}

			//Allocate space for the value and terminator
			
			value = malloc(count+1);
			//Copy the value from the buffer
			memcpy(value,buffer,count);
			value[count] = '\0';
			token.value = value;
			token.type = silentIntegerToken;
			if(floatVal){token.type = silentFloatToken;}
            //i += count-1;
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
		else if(isspace(source[i]))
		{
			continue;
		}

		else{
			continue;
		}
		tokens[*tokenCount] = token;
		//printf("%s\n",tokens[*tokenCount].value);
		*tokenCount += 1;
	}
	//printf("%s\n",tokens[0].value);
	return tokens;
}