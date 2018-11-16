#pragma once
#include <iostream>
#include <vector>
#include "SilentTokenizer.hpp"
namespace Silent
{
    enum class SilentStatementType
    {
        VarInit,
        Expression,
        If,
        Else,
        While,
        For
    };

    enum class SilentPrimitives
    {
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32,
        int64,
        uint64,
        float32,
        float64,
        string,
        pointer,
        null,
        undefined
    };

    enum class SilentOperandType
    {
        Assign,
        Add,
        Subtract,
        Multiply,
        Divide,
        Equal,
        NotEqual,
        Larger,
        LargerOrEqual,
        Smaller,
        SmallerOrEqual,
        And,
        Or,
        Xor,
        Not,
        FunctionCall,  
        Number,
        String,
        Variable
    };

    enum class SilentExpressionType
    {
        Arithmetic,
        Logical,
        Comparison,
        Data,
        Memory
    };

    enum class ParentType
    {
        None,
        LocalScope,
        Namespace
    };

    typedef struct SilentVariable SilentVariable;
    typedef struct SilentStructure SilentStructure;
    typedef struct SilentFunction SilentFunction;
    typedef struct SilentOperand SilentOperand;
    typedef struct SilentLocalScope SilentLocalScope;
    typedef struct SilentStatement SilentStatement;
    typedef struct SilentNamespace SilentNamespace;


    typedef struct SilentDataType
    {
        union
        {
            SilentStructure* type;
            SilentPrimitives primitive;
        };
        bool isPrimitive;
    }SilentDataType;

    typedef struct SilentFunctionCall
    {
        SilentFunction* function;
        std::vector<SilentOperand*> arguments;

    }SilentFunctionCall;

    typedef struct SilentVariable
    {
        unsigned long long size;
        unsigned long long localPos;
        unsigned long long basePtr;
        std::string name;
        //SilentOperand* expresion;
        SilentDataType type;
        bool initialised;
    }SilentVariable;

    typedef struct SilentStructure
    {
        SilentLocalScope* variables;
        std::string name;
        unsigned long long size;
        bool initialised;
    }SilentStructure;

    typedef struct SilentLocalScope
    {
        std::vector<SilentVariable*> variables;
        std::vector<SilentStatement*> statements;
        ParentType parentType;
        union
        {
            SilentLocalScope* scopeParent;
            SilentNamespace* namespaceParent;
        };
    }SilentLocalScope;

    typedef struct SilentFunction
    {
        //SilentLocalScope* parameters;
        SilentLocalScope* scope;
        uint64 parameterCount;
        uint64 parameterSize;
        std::string name;
        unsigned long long returnTypePtr;
        unsigned long long returnSize;
        SilentDataType returnType;
        bool initialised;
        //bool hasParent;
        //SilentNamespace* parent;
    }SilentFunction;

    typedef struct SilentOperand
    {
        SilentExpressionType expressionType;
        SilentOperandType type;
        SilentOperand *left, *right;
        union
        {
            SilentToken* token;
            SilentVariable* variable;
            SilentFunctionCall* functionCall;
        };
    }SilentOperant;

    typedef struct SilentIfStatement
    {
        SilentOperand* expression;
        SilentLocalScope* scope;
    }SilentIfStatement;

    typedef struct SilentWhileLoop
    {
        SilentOperand* expression;
        SilentLocalScope* scope;
    }SilentWhileLoop;

    typedef struct SilentStatement
    {
        SilentLocalScope* parentScope;
        SilentStatementType type;
        union
        {
            SilentIfStatement* ifStatement;
            SilentWhileLoop* whileLoop;
            SilentVariable* variable;
            SilentOperand* expression;
        };
        
    }SilentStatement;

    typedef struct SilentNamespace
    {
        SilentNamespace* parent;
        bool hasParent;
        std::vector<SilentNamespace*> namespaces;
        std::vector<SilentFunction*> functions;
        std::vector<SilentStructure*> types;
        SilentLocalScope* globals;
        std::string name;
        uint64 globalPos;
    }SilentNamespace;

    class SilentParser
    {
        public:
        SilentParser(); ~SilentParser();
        bool SilentParse(std::vector<Silent::SilentToken> tokens);
        SilentNamespace* GetGlobalNamespace();

        private:

        void ErrorMsg(std::string msg);

        SilentNamespace* GetNamespace(std::string name);
        SilentStructure* GetStructure(std::string name);
        SilentDataType GetType(std::string name);
        SilentFunction* GetLocalFunction(std::string name);
        SilentFunction* GetFunction(std::string name);
        uint64 GetTypeSize(std::string name);
        uint64 GetLocalPos(SilentLocalScope &scope);
        //SilentVariable* GetLocalVariable(
        //    SilentLocalScope &scope, std::string name);
        SilentVariable* GetVariable(SilentLocalScope &scope, std::string name);
        SilentVariable* GetVariable(SilentNamespace &scope, std::string name);

        bool IsPrimitive(std::string name);
        bool IsValidType(std::string name);

        void NextToken();
        void PreviousToken();
        SilentToken PeakToken();
        bool AcceptToken(SilentTokenType type);
        bool ExpectToken(SilentTokenType type, std::string msg);
        
        SilentFunctionCall* ParseFunctionCall(SilentLocalScope &scope);
        SilentOperand* ParseFactor(SilentLocalScope &scope);
        SilentOperand* ParseTerm(SilentLocalScope &scope);
        SilentOperand* ParseSum(SilentLocalScope &scope);
        SilentOperand* ParseLogic(SilentLocalScope &scope);
        SilentOperand* ParseComparison(SilentLocalScope &scope);
        SilentOperand* ParseExpression(SilentLocalScope &scope); 
        SilentStatement* ParseStatement(SilentLocalScope &scope);
        SilentVariable* ParseVariable(
            SilentLocalScope &scope, bool init, bool expectEnd);
        SilentStructure* ParseStruct(SilentNamespace &scope);
        SilentLocalScope* ParseParameters();
        void ParseLocalScope(SilentLocalScope &scope);
        void ParseIfStatement(SilentLocalScope &scope);
        void ParseFunction(SilentNamespace& scope);
        SilentNamespace* ParseNamespace(SilentNamespace &scope);

        SilentNamespace* globalNamespace;
        SilentToken ct; //Current token
        uint64 tokenCursor;
        uint64 globalVarPointer;
        std::vector<SilentNamespace*> accessibleNamespaces;
        std::vector<Silent::SilentToken> *tokensPtr;

        
    };
}