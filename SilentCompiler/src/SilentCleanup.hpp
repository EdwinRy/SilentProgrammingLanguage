#pragma once
#include "SilentParser.hpp"
#include <vector>

namespace Silent
{
    class SilentCleaner
    {
        private:
        void FreeVariable(SilentVariable* variable);
        void FreeLocalScope(SilentLocalScope* scope);
        void FreeStructure(SilentStructure* structure);
        void FreeFunction(SilentFunction* function);
        void FreeNamespace(SilentNamespace* scope);
        public:
        SilentCleaner(){}
        void CleanupParser(SilentParser* parser);
    };
}
