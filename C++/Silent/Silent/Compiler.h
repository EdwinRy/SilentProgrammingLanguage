#pragma once
#include <iostream>
#include <vector>

namespace Silent {

	class Compiler {

    private:

        class s_Instruction 
        {

        };

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

        class s_Function
        {

        };

        class s_GlobalScope {
            std::vector<s_Function> functions;
            std::vector<s_Boolean> globalBooleans;
            std::vector<s_Integer> globalIntegers;
            std::vector<s_Char> globalChars;
        };

	public:
        Compiler();
        void compile(const char *filePath, const char *outputPath);
		 
	};
}