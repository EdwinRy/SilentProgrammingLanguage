#pragma once
#include "STokenizer.hpp"
#include <iostream>
#include <vector>

namespace Silent
{
    class Node
    {
        public:

        enum class Type
        {
            Integer,
            Identifier,
            Primitive,
            Namespace

        }type;

        std::string value;
        std::vector<Node*> children;

    };

    class Parser
    {

        // Parser helper methods
        public:
        Parser();
        Node* Parse(std::vector<Silent::Token> tokensPtr);
        void ErrorMsg(std::string msg);
        void WarningMsg(std::string msg);

        Token GetToken();
        Token NextToken();
        Token NextToken(int offset);
        Token PeakToken();
        Token PeakToken(int offset);
        Token PreviousToken();
        Token PreviousToken(int offset);
        Token Match(TokenType type);
        
        private:
        unsigned long long errorCount;
        Token ct;
        unsigned long long tokenCursor;
        std::vector<Silent::Token> *tokensPtr;

        // Production rules
        public:

        //TOKENS
        Node* ParseIdentifier();
        Node* ParseInteger();//
        Node* ParseFloat();//
        Node* ParseNamespaceName();//
        Node* ParseTypeName();//
        Node* ParseExpressionLhs();//
        Node* ParseName();//
        Node* ParsePrimitive();//
        Node* ParseType();//
        Node* ParseLiteral();//

        //EXPRESSIONS
        Node* ParseExpression();//
        Node* ParseAssignment();//
        Node* ParseAssignmentOperator();//

        Node* ParseLHS();//
        Node* ParseArrayAccess();//

        Node* ParseConditionalExpression();//
        Node* ParseConditionalOperator();//

        Node* ParseLogicalExpression();//
        Node* ParseLogicalOperator();//

        Node* ParseComparisonExpression();//
        Node* ParseComparisonOperator();//

        Node* ParseShiftExpression();//
        Node* ParseShiftOperator();//

        Node* ParseAddition();//
        Node* ParseAdditionOperator();//

        Node* ParseMultiplication();//
        Node* ParseMultiplicationOperator();//

        Node* ParseFactor();//
        Node* ParseFunctionCall();//

        //SCOPES
        Node* ParseScope();//
        Node* ParseNamespaceScope();
        Node* ParseNamespaceStatements();
        Node* ParseNamespaceStatement();//


        //TYPES

        //DECLARATIONS
        Node* ParseNamespaceDeclaration();//


        
    };
}