#pragma once
#include "SilentParser.hpp"
#include <vector>
#include <unordered_map>
namespace Silent
{
    class SilentCodeGenerator
    {
        public:
        void Compile(SilentParser* parser);
        std::string GetOutput();

        private:
        std::string GenScopeName(std::string id);
        std::string CompileExpression(SilentOperand &expression);
        std::string CompileFunction(SilentFunction &func);
        std::string CompileNamespace(SilentNamespace &scope);
        
        std::vector<SilentNamespace*> namespaces;
        std::unordered_map<std::string, uint64> symTable;
        std::string currentFunction;
        std::string code;
        uint64 cp; //code pointer
    };
}