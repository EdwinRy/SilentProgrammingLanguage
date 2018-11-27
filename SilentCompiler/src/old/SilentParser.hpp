#include <iostream>
#include <vector>
#include "SilentTokenizer.hpp"
#include "SilentTypes.hpp"
#pragma once
namespace Silent
{
    class Parser
    {
        public:
        Parser();
        Namespace* Parse(std::vector<Silent::Token> tokens);

        void ErrorMsg(std::string msg);
        //Finding symbols within the parse tree
        Namespace* FindNamespace(std::string name);
        Structure* FindStructure(std::string name);
        DataType FindType(std::string name);
        Function* FindFunction(std::string name);
        Function* FindFunctionLocally(std::string name);
        Variable* GetVariable(LocalScope* scope, std::string name);

        //Dealing with tokens
        void NextToken();
        void PreviousToken();
        Token PeekToken();

        //Parsing data
        Namespace *globalNamespace;
        Token ct; //Current token
        uint64 tokenCursor;
        uint64 globalVarPointer;
        std::vector<Namespace*> accessibleNamespaces;
        std::vector<Silent::Token> *tokensPtr;
        Function* currentFunction;
        DataType currentDataType;
    };
}