#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>

namespace Silent {

	class Compiler {

    private:

        class s_Boolean
        {
            bool value;

            bool referenced;
            bool constant;
        };
        
        class s_Integer
        {
            int value;

            bool referenced;
            bool constant;
        };

        class s_Char 
        {
            char* value;

            bool referenced;
            bool constant;
        };

        class s_Instruction
        {

        };

        class s_Function
        {

        };

        class s_Structs {

        };

        class s_GlobalScope {
            std::vector<s_Structs> structures;
            std::vector<s_Function> functions;
            std::vector<s_Char> globalChars;
        };

	public:
        Compiler();
        void compile(const char *filePath, const char *outputPath);
		 
	};
}