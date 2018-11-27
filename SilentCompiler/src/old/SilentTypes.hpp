#include "SilentParser.hpp"
#pragma once
namespace Silent
{
    class DataType
    {
        public:

    };

    class Value
    {

    };

    
    class Variable
    {

    };

    class Reference
    {

    };

    class Structure
    {

    };

    class LocalScope
    {

    };

    class FunctionCall
    {
        public:
        void Parse(Parser parser);
    };

    class Function
    {
        public:
        void Parse(Parser parser);
    };

    class Operand
    {
        public:
        void Parse(Parser parser);

        private:
        Operand* ParseExpression();
        Operand* ParseComparison();
        Operand* ParseLogic();
        Operand* ParseSum();
        Operand* ParseTerm();
        Operand* ParseFactor();
    };

    class Statement
    {
        public:
        void Parse(Parser parser);
    };

    class IfStatement : Statement
    {
        public:
        void Parse(Parser parser);
    };

    class WhileLoop : Statement
    {
        public:
        void Parse(Parser parser);
    };

    class Namespace
    {
        public:
        void Parse();

    };
}