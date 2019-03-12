#pragma once
#include "SParser.hpp"
#include <iostream>
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
        std::string identifier;
        ScopeResolution* scopeResolution;

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
        public:
        enum class Type
        {
            Namespace,
            Class,
            Function,
            Variable


        }nodeType;

        union
        {
            Type *type;
            Function *function;
            Variable *variable;
            Namespace *module;
        };
    };
};