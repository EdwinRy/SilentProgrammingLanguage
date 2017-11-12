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
//Helper functions
char silentTestLetter(char character)
{
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

}

char silentTestNumber(char character)
{
	string numbers = "1234567890";
}

//Compile structures
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
	string rawSource = fr.readAllText(path);
	vector<silentToken> tokens = silentTokenize(&rawSource);

}
