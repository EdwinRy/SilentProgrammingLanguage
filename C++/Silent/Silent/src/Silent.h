#pragma once

//#include <iostream>


void printNoArgs() {

	using namespace std;

	cout << "No arguments provided" << endl;
	cout << "input -h for help" << endl;
}

void printHelp() {

	using namespace std;

	cout << "-c YourFile.silent to compile the program" << endl;
	cout << "-o to set the output filename and directory" << endl;

}

int main() {
	printNoArgs();
}
  /*
int main(int argc, char *argv[]) {
	//if argc == 2 there's only 1 argument passed
	if (argc == 2) {
		printNoArgs();
	}

	else {
		for(int i = 2;i < argc;i++){
			if (argv[3] == "-h") {
				printHelp();
			}

			if (argv[i] == "-c") {

			}
		}
		
	}
	
}
	  */
