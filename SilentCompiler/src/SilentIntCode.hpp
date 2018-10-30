#pragma once
#include "SilentParser.hpp"
namespace Silent
{

    //SilentIntCode* SilentTransform(SilentParserInfo* parsedCode);

    class SilentIntCode
    {
        public:
        bool SilentTransform(SilentParser* parser);

        private:
        std::string TypeToString(SilentPrimitives dt);
        void TransformExpression(SilentOperand &expression);
        void TransformVariable(SilentVariable &var);
        void TransformLocalScope(SilentLocalScope &scope);
        void TransformFunction(SilentFunction &function);
        void TransformStructure(SilentStructure &structure);
        void TransformNamespace(SilentNamespace &scope);
        std::vector<std::string> SplitString(std::string str, char splitChar);

        std::string code;
    };
}