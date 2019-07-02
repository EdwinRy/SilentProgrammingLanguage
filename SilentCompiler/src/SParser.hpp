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
            Program,
            Digits,
            String,
            Integer,
            Float,
            Identifier,
            Primitive,
            Function,
            Namespace,
            NamespaceName,
            Name,
            NamespaceScope,
            NamespaceStatements,
            NamespaceStatement,
            LocalScope,
            LocalStatements,
            LocalStatement,
            Sign,
            LHS,
            Expression,
            Assignment,
            AssignmentOperator,
            ConditionalExpression,
            ConditionalOperator,
            LogicalExpression,
            LogicalOperator,
            ComparisonExpression,
            ComparisonOperator,
            ShiftExpression,
            ShiftOperator,
            Addition,
            AdditionOperator,
            Multiplication,
            MultiplicationOperator,
            Factor,
            Literal,
            FunctionName,
            Parameters,
            Parameter,
            TypeSpecifier,
            TypeName
        }type;

        union 
        {
            
        };
        

        Node(){}
        Node(Type type);

        std::string value;
        std::vector<Node*> children;

    };

    class Parser
    {
        // Parser helper methods
        public:
        Parser();
        Node* Parse(std::vector<Silent::Token> tokensPtr);

        // private:
        void ErrorMsg(std::string msg);
        void WarningMsg(std::string msg);

        Token GetToken();
        Token NextToken();
        Token NextToken(int offset);
        Token PeakToken();
        Token PeakToken(int offset);
        Token PreviousToken();
        Token PreviousToken(int offset);
        bool Match(TokenType type);
        void DeleteNode(Node* node);
        
        unsigned long long errorCount;
        Token ct;
        unsigned long long tokenCursor;
        std::vector<Silent::Token> *tokensPtr;

        // Production rules

        //TOKENS
        Node* ParseDigits();
        Node* ParseString();
        Node* ParseIdentifier();
        Node* ParseInteger();
        Node* ParseSign();
        Node* ParseFloat();
        Node* ParseNamespaceName();
        Node* ParseFunctionName();
        Node* ParseTypeName();
        // Node* ParseExpressionLhs();//
        Node* ParseName();
        Node* ParsePrimitive();
        Node* ParseLiteral();
        Node* ParseTypeSpecifier();

        //EXPRESSIONS
        Node* ParseExpression();
        Node* ParseAssignment();
        Node* ParseAssignmentOperator();

        Node* ParseLHS();
        Node* ParseArrayAccess();//

        Node* ParseConditionalExpression();
        Node* ParseConditionalOperator();

        Node* ParseLogicalExpression();
        Node* ParseLogicalOperator();

        Node* ParseComparisonExpression();
        Node* ParseComparisonOperator();

        Node* ParseShiftExpression();
        Node* ParseShiftOperator();

        Node* ParseAddition();
        Node* ParseAdditionOperator();

        Node* ParseMultiplication();
        Node* ParseMultiplicationOperator();

        Node* ParseFactor();
        Node* ParseFunctionCall();//

        //SCOPES
        Node* ParseNamespaceScope();
        Node* ParseNamespaceStatements();
        Node* ParseNamespaceStatement();
        Node* ParseLocalScope();
        Node* ParseLocalStatements();
        Node* ParseLocalStatement();


        //TYPES

        //DECLARATIONS
        Node* ParseFunctionDeclaration();
        Node* ParseParameters();
        Node* ParseParameter();
        Node* ParseFunctionProperties();//
        Node* ParseVariableDeclaration();//
        Node* ParseNamespaceDeclaration();
    };

    class Program : Node
    {

    };
}