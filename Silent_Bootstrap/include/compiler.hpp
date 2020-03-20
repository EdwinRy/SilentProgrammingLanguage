#include "parser.hpp"
#include "lexer.hpp"
#pragma once

namespace Silent
{
    class Compiler
    {
        public:
        char* GetBytes(char* source);

        private:
        Parser parser;
    };
}