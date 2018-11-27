#pragma once
#include <iostream>
namespace Silent
{    
    class Compiler
    {
        public:
        Compiler();
        std::string Compile(std::string source);
    };
}