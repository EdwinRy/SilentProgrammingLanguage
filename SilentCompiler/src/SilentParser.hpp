#pragma once
#include <iostream>
#include <vector>
#include "SilentTokenizer.hpp"
#include "SilentStructures.hpp"
namespace Silent
{
    class Parser
    {
        public:
        Parser(); ~Parser();
        bool Parse(std::vector<Silent::Token> tokens);
        Structures::Namespace *GetGlobalNamespace();

        void ErrorMsg(std::string msg);

        Token NextToken();
        Token PreviousToken();
        Token PeakToken();
        Token GetToken();
        Token ExpectToken(TokenType type, std::string errMsg);
        Token ExpectNextToken(TokenType type, std::string errMsg);

        Structures::DataType GetType(std::string id);
        Structures::DataType GetType() {return GetType(ct.value);}

        Structures::Variable* GetVariable(
            Structures::LocalScope &scope, std::string id);

        Structures::Variable* GetVariable(Structures::LocalScope &scope)
        {return GetVariable(scope, ct.value);}

        std::vector<Structures::Variable*> GetMember(Structures::Variable* parent);

        Structures::Variable* GetVariable(
            Structures::Namespace &scope, std::string id);

        Structures::Variable* GetVariable(Structures::Namespace &scope)
            {return GetVariable(scope, ct.value);}

        Structures::Function* GetFunction(std::string id);
        Structures::Function* GetFunction() {return GetFunction(ct.value);}

        void PushNewNamespace(Structures::Namespace* scope);
        void PopNewNamespace();

        Structures::DataType currentType;
        Structures::Function* currentFunction;

        private:
        uint64 errorCount;
        Structures::Namespace* globalNamespace;
        Token ct; //Current token
        uint64 tokenCursor;
        uint64 globalVarPointer;
        std::vector<Structures::Namespace*> accessibleNamespaces;
        std::vector<Silent::Token> *tokensPtr;
    };
}