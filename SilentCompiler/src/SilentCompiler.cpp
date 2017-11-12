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

//Tokenizer






string* silentCompile(string path)
{

	fileReader fr;
	string rawSource;

}
