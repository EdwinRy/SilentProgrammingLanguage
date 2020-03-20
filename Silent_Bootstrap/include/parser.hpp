#include <iostream>
#include <vector>
#include "lexer.hpp"
#pragma once

namespace Silent
{
    namespace ParserStruct
    {
        enum class NodeType
        {

        };

        class Node
        {
            std::vector<Node> children;
            Node* parent;
            Token value;
        };
    }

    class Parser
    {
        public:
        ParserStruct::Node GenerateAst(char* source);

        private:
        ParserStruct::Node ast;
        Lexer lexer;
    };
}