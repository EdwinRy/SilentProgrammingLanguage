#include "SParser.hpp"
#include <vector>
#pragma once

namespace Silent
{
    class CodeGenerator
    {
        public:
        char* GenBytecode(Program* ast);

        private:
        bool TraverseNode(Program* node);
        std::vector<char> code;
        char* bytecodeOut = NULL;
    };
}