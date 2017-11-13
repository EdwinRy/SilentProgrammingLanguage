#include "SilentCompiler.h"
#include <iostream>
#include <fstream>
#include <vector>
//File reader

using namespace std;

class fileReader{
	public:
	fileReader()
	{

	}
	string readAllText(string path)
	{
		string output;

		return output;
	}
	vector<string> readAllLines(string path)
	{
		vector<string> output;
		ifstream source(path);
		//if(source.is_open())
		return output;
	}
	string readLine(string path)
	{
		string output;

		return output;
	}
};

//Compiler structures



//Helper functions
char silentTestLetter(char character)
{
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

}

char silentTestNumber(char character)
{
	string numbers = "1234567890";
}

char silentTestWhitespace(char character)
{

}

//Tokenizer

vector<silentToken> silentTokenize(string* source)
{

}

//Parser


//Transform


//Code Generation




string* silentCompile(string path)
{

	fileReader fr;
	string rawSource = fr.readAllText(path);//no AST
	vector<silentToken> tokens = silentTokenize(&rawSource);

}
