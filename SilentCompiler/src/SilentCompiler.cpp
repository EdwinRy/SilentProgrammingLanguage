#include "SilentCompiler.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
//File reader
class fileReader{
	public:
	fileReader()
	{

	}

	//Read all characters from a file
	string readAllText(string path)
	{
		string output;
		ifstream file(path);
		output.assign((istreambuf_iterator<char>(file)),
			(istreambuf_iterator<char>()));
		return output;
	}

	//Read all lines from a file
	vector<string> readAllLines(string path)
	{
		vector<string> output;
		ifstream file(path);
		//if(source.is_open())
		return output;
	}

	//Read a single line from a file
	string readLine(string path)
	{
		string output;
		ifstream file(path);

		return output;
	}
};

//Compiler structures
enum silentTokenType
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

	
	//Access
	silentPublicToken,
	silentPrivateToken,
	silentProtectedToken,

	//Data
	silentByteToken,
	silentIntegerToken,
	silentFloatToken,
	silentLongToken,
	silentDoubleToken,
	silentStringToken,
};

typedef struct silentToken
{
	silentTokenType type;
	std::string value;

}silentToken;


enum silentNodeType
{
	//Value
	silentCharacterNode,
	silentStringNode,
	silentIntegerNode,
	silentLongNode,
	silentFloatNode,
	silentDoubleNode,

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
 
};


typedef struct silentValueNode
{
	silentNodeType type;
	string value;
}silentValueNode;

typedef struct silentExpressionNode
{
	silentNodeType type;
	silentValueNode parameters[2];

}silentExpressionNode;

typedef struct silentFunctionNode
{
	vector<silentExpressionNode> expressions;
}silentFunctionNode;

//Helper functions
//Test whether the passed token is a character
char silentTestLetter(char character)
{
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for(int i = 0; i < letters.length(); i++)
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
	string numbers = "1234567890";
	for(int i = 0; i < numbers.length(); i++)
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

vector<silentToken> silentTokenize(string source)
{
	long currentChar = 0;
	long currentLine = 0;
	vector<silentToken> tokens;

	for(int i = 0; i < source.length(); i++)
	{
		silentToken token;

		if((source[i] == *"(") || (source[i] == *")"))
		{
			token.type = silentParenthesToken;
			token.value = source[i];
		}

		if(source[i] == *"\"")
		{
			token.type = silentQuotationToken;
			token.value = "";
			for(int j = 1; source[i+j] != *"\"";j++)
			{
				token.value += source[i+j];
			}
		}

		if(silentTestWhitespace(source[i]))
		{
			continue;
		}

		tokens.push_back(token);
	}




	return tokens;
}

//Parser


//Transform


//Code Generation




string* silentCompile(string path)
{

	fileReader fr;
	string rawSource = fr.readAllText(path);//no AST
	vector<silentToken> tokens = silentTokenize(rawSource);

}
