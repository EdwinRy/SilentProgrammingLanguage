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
        bool operator==(const ScopeResolution& rhs) const;
        ScopeResolution& operator--();

    };

    class SymTableNode
    {
        public:
        ScopeResolution scopeReference;
        Types::Node node;
        std::vector<SymTableNode*> children;
        static thread_local 
            std::unordered_map<ScopeResolution, SymTableNode*> symTable;
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

        private:
        unsigned long long errorCount;
        Token ct;
        unsigned long long tokenCursor;
        std::vector<Silent::Token> *tokensPtr;
    };
}

namespace std
{
    template<>
    struct hash<Silent::ScopeResolution>
    {
        std::size_t operator()(const Silent::ScopeResolution& sr) const
        {
            return std::hash<std::string>{}(sr.scopeFormatted);
        }
    };
}