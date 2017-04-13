#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>


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

    struct s_Char 
    {
        char* value;

        bool referenced;
        bool constant;
    };

    struct s_Instruction
    {

    };

    struct s_Function
    {

    };

    struct s_Structs {

    };

    struct s_GlobalScope {
        std::vector<s_Structs> structures;
        std::vector<s_Function> functions;
        std::vector<s_Char> globalChars;
    };

public:
    Compiler();
    void compile(const char *filePath, const char *outputPath);
		 
};