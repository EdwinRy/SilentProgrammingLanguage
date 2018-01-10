#include "SilentTokenizer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Turn the sourcode into tokens array
silentToken* silentTokenize(char* source, int* tokenCount)
{
	//Current character
	long currentChar = 0;
	//Current line
	long currentLine = 0;
	//Character buffer for parsing
    char buffer[255];
	//Array of tokens
	silentToken* tokens = malloc(strlen(source)*sizeof(silentToken));
	if(tokens == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
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
			//Allocate 2 bytes for the string
			token.value = malloc(2);
			//Assign first character to the parentheses
			token.value[0] = ';';
			//Null terminate the string
			token.value[1] = '\0';
		}

		//Test for fullstop
		else if(source[i] == '.')
		{
			token.type = silentFullstopToken;
			token.value = ".";
		}

		//Test for comma
		else if(source[i] == ',')
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
				buffer[count] = source[i+count];count+=1;
			}
			//Allocate space for the value and terminator
			value = malloc(count+1);
			//Copy the value from the buffer
			memcpy(value,buffer,count);
			value[count] = '\0';
			token.value = value;
			token.type = silentIdentifierToken;
			//Test for language tokens
			if(strcmp(token.value, "func")==0)
			{
				token.type = silentFunctionToken;
			}
			else if(strcmp(token.value, "struct")==0)
			{
				token.type = silentStructToken;
			}
			else if(strcmp(token.value, "var")==0)
			{
				token.type = silentVariableToken;
			}
			else if(strcmp(token.value, "int")==0)
			{
				token.type = silentIntegerToken;
			}
			else if(strcmp(token.value, "float")==0)
			{
				token.type = silentFloatToken;
			}
			else if(strcmp(token.value, "long")==0)
			{
				token.type = silentLongToken;
			}
			else if(strcmp(token.value, "double")==0)
			{
				token.type = silentDoubleToken;
			}
			else if(strcmp(token.value, "string")==0)
			{
				token.type = silentStringToken;
			}
			
			//Set the token to be an identifier if it's not used by the language
			else
			{
				token.type = silentIdentifierToken;
			}
			i += count-1;
		}

		//Test for numbers
		else if(isdigit(source[i]))
		{
			char* value;
			char floatVal = 0;
			char count = 0;
			while(isdigit(source[i]) || source[i] == '.')
			{
				if(source[i] == '.')
				{
					floatVal = 1;
				}
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
			token.type = silentNumberToken;
			if(floatVal){token.type = silentDecimalToken;}
            i += count;
			i--;
		}

		//Test for text
		/*
		else if(source[i] == '\"')
		{
			token.type = silentQuotationToken;
			token.value = "";
			for(int j = 1; source[i+j] != '\"';j++)
			{
				token.value += source[i+j];
			}
		}*/

		//Test for whitespaces
		else if(isspace(source[i]))
		{
			continue;
		}

		else{
			continue;
		}
		tokens[(*tokenCount)] = token;
		*tokenCount += 1;
	}

	//Closing tokenizer
	free(source);
	return tokens;
}