#include <iostream>
#include "Compiler.h"
#include "Interpreter.h"

void PrintNoArgs() 
{
	using namespace std;

	cout << "No arguments passed" << endl;
	cout << "input -h for help" << endl;

}

void PrintHelp() 
{
	using namespace std;

	cout << "-c YourProgram.silentclass to compile your program" << endl;
	cout << "-o to set the output filename and directory" << endl;
    cout << "-e YourProgram.silent to execute the file" << endl;
	cout << "silent YourProgram.silent to run the program" << endl;
}

int main(int argc, char *argv[]) 
{


	using namespace Silent;

	char *inputFile;
	char *outputFile;

	if (argc == 1) 
    {
		PrintNoArgs();
	}

	else 
    {
		for (int i = 1; i < argc; i++) 
        {

			if (argv[i] == "-h") 
            {
				PrintHelp();
				break;
			}

			if (argv[i] == "-c") 
            {
				inputFile = argv[i + 1];
				outputFile = "default";
                Compiler *compiler = new Compiler();

                compiler->compile(inputFile, outputFile);
			}

            if (argv[i] == "-e") 
            {
                inputFile = argv[i + 1];
                Interpreter *interpreter = new Interpreter();

                interpreter->interpret(inputFile);
            }

		}

		

	}

	return 0;
}