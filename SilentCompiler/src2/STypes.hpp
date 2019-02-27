#pragma once
#include "SParser.hpp"
#include <vector>
namespace Silent
{
    typedef class Parser parser;
    typedef class ScopeResolution ScopeResolution;
}

namespace Silent::Types
{

    class Namespace
    {
        public:
        static void Parse(Parser &parser, ScopeResolution parent);
    };

    class Function
    {

    };

    class Variable
    {

    };

    class Type
    {

    };

    class Node
    {
        enum class Type
        {
            Class,
            Function,
            Variable

        }nodeType;

        union
        {
            Type *type;
            Function *function;
            Variable *variable;
        };
    };
};