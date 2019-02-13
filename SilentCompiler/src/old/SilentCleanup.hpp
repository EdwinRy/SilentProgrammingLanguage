#pragma once
#include "SilentStructures.hpp"
#include "SilentParser.hpp"
#include <vector>

namespace Silent
{
    class SilentCleaner
    {
        private:
        void FreeVariable(Structures::Variable* variable);
        void FreeLocalScope(Structures::LocalScope* scope);
        void FreeStructure(Structures::Structure* structure);
        void FreeFunction(Structures::Function* function);
        void FreeNamespace(Structures::Namespace* scope);
        public:
        SilentCleaner(){}
        void CleanupParser(Parser* parser);
    };
}
