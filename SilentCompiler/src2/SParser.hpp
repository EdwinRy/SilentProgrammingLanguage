#pragma once
#include "STypes.hpp"
#include "STokenizer.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
namespace Silent
{

    class ScopeResolution
    {
        public:
        ScopeResolution(){}
        ScopeResolution(std::string scope);
        ScopeResolution(std::vector<std::string> scope);

        std::vector<std::string> scopeList;
        std::string scopeFormatted;

        ScopeResolution& operator+=(std::string rhs);
        ScopeResolution& operator--();

    };

    class SymTableNode
    {
        public:
        ScopeResolution scopeReference;
        Types::Node node;
        std::vector<SymTableNode> children;
        static std::unordered_map<ScopeResolution, SymTableNode*> symTableData;
    };

    class Parser
    {
        public:
        Parser();
        void ErrorMsg(std::string msg);

        Token GetToken();
        Token NextToken();
        Token PeakToken();
        Token PreviousToken();
        
        SymTableNode Parse(std::vector<Silent::Token> tokens);
        SymTableNode symTable;

        std::unordered_map<ScopeResolution, SymTableNode*>& GetSymData()
        {
            return symTable.symTableData;
        }

        private:
        unsigned long long errorCount;
        Token ct;
        unsigned long long tokenCursor;
        std::vector<Silent::Token> *tokensPtr;
    };
}