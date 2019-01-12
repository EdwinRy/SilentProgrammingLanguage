#pragma once
#include <iostream>
#include <vector>

namespace Silent{typedef class Parser parser;}
namespace Silent{typedef class CodeGenerator CodeGenerator;}
namespace Silent::Structures
{
    enum class Primitives
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
        reference,
        null,
        undefined
    };
    

    typedef class Node Node;
    typedef class LocalScope LocalScope;
    typedef class Statement Statement;
    typedef class Namespace Namespace;
    typedef class Function Function;
    typedef class WhileLoop WhileLoop;
    typedef class IfStatement IfStatement;
    typedef class Operand Operand;
    typedef class Structure Structure;
    typedef class Variable Variable;
    typedef class FunctionCall FunctionCall;
    typedef class Value Value;
    typedef class DataType DataType;

    class Node
    {
        public:
        enum class Type
        {
            DataType,
            Value,
            FunctionCall,
            Variable,
            Reference,
            Structure,
            Primitive,
            LocalScope,
            Function,
            Operand,
            IfStatement,
            WhileLoop,
            Statement,
            Namespace,
            Members,
            Undefined
        };

        Type nodeType;
        union 
        {
            Namespace* namespaceScope;
            Statement* statement;
            WhileLoop* whileLoop;
            IfStatement* ifStatement;
            Operand* operand;
            Function* function;
            LocalScope* localScope;
            Structure* structure;
            Variable* variable;
            //Primitive* primitive;
            Primitives primitiveType;
            FunctionCall* functionCall;
            Value* value;
            DataType* dataType;
            std::vector<Variable*>* members;
        };
    };

    struct DataType
    {
        union
        {
            Structure* structure;
            Primitives primitive;
        };

        enum class Type
        {
            Structure,
            Primitive,
            Undefined
        };

        Type dataType;
        unsigned long long size;
    };

    struct Value
    {
        DataType type;
        std::string data;
    };

    class Variable
    {
        public:
        bool Parse(Parser &parser, LocalScope &scope, bool isParam);
        bool Compile(CodeGenerator &cg);
        unsigned long long GetSize(){return size;}
        void SetLocalPos(unsigned long long newPos){localPos = newPos;}
        unsigned long long GetLocalPos(){return localPos;}
        std::string GetId(){return identifier;}
        DataType GetType(){return type;}

        bool isReference;

        private:
        unsigned long long size;
        unsigned long long localPos;
        std::string identifier;
        DataType type;
        bool initialised;
    };

    class Structure
    {
        public:
        //Structure();
        bool Parse(Parser &parser, Namespace& scope);
        //bool Compile(CodeGenerator &cg);
        std::string GetId(){return identifier;}
        unsigned long long GetSize(){return size;}
        std::vector<Variable*> members;

        private:
        // LocalScope* members;
        unsigned long long size;
        bool initialised;
        std::string identifier;
    };

    class LocalScope
    {
        public:
        LocalScope(){varSize = 0;}
        bool Parse(Parser &parser);
        bool ParseParameters(Parser &parser, Function &function);
        bool Compile(CodeGenerator &cg);
        unsigned long long GetLocalPos(){return this->varSize;}
        Variable* GetVariable(std::string id);
        std::vector<Statement*> *GetStatements(){return &statements;}
        std::vector<Variable*> *GetVariables(){return &variables;}

        union
        {
            LocalScope* localScope;
            Namespace* namespaceScope;
            Structure* structure;
            Function* function;
        }parent;

        enum class ParentType
        {
            LocalScope,
            Namespace,
            Structure,
            Function
        };

        ParentType parentType;

        private:
        unsigned long long varSize;
        std::vector<Variable*> variables;
        std::vector<Statement*> statements;
    };

    class Function
    {
        public:
        //Function();
        bool Parse(Parser &parser, Namespace &scope);
        bool Compile(CodeGenerator &cg);
        std::string GetId(){return identifier;}
        unsigned long long GetParameterCount(){return parameterCount;}
        unsigned long long GetParameterSize(){return parameterSize;}
        Variable* GetVariable(std::string id){return scope->GetVariable(id);}
        std::vector<Variable*> *GetVariables(){return scope->GetVariables();}
        DataType GetReturnType(){return returnType;}
        LocalScope* GetScope(){return scope;}

        private:
        LocalScope* scope;
        unsigned long long parameterCount;
        unsigned long long parameterSize;
        DataType returnType;
        std::string identifier;
        bool initialised;
    };

    struct FunctionCall
    {
        Function* function; //Function
        std::vector<Operand*> arguments; //Operands
    };

    class Operand
    {
        public:
        //Operand();
        bool Parse(Parser &parser, LocalScope &scope);
        static FunctionCall* ParseFunctionCall(
            Parser &parser, LocalScope &scope);
        static Operand* ParseFactor(Parser &parser, LocalScope &scope);
        static Operand* ParseTerm(Parser &parser, LocalScope &scope);
        static Operand* ParseSum(Parser &parser, LocalScope &scope);
        static Operand* ParseLogic(Parser &parser, LocalScope &scope);
        static Operand* ParseComparison(Parser &parser, LocalScope &scope);
        static Operand* ParseExpression(Parser &parser, LocalScope &scope);
        Operand* GetLeft(){return left;}
        Operand* GetRight(){return right;}
        bool Compile(CodeGenerator &cg);

        enum class OperandType
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
            Variable,
            Members
        };

        enum class ExpressionType
        {
            Arithmetic,
            Logical,
            Comparison,
            Data,
            Memory,
            Null
        };

        Node value;
        ExpressionType expressionType;
        OperandType operandType;

        private:
        Operand *left, *right;
    };

    class Statement
    {
        public:
        bool Parse(Parser &parser, LocalScope &scope);
        bool Compile(CodeGenerator &cg);

        enum class Type
        {
            VarInit,
            Expression,
            FunctionCall,
            If,
            While,
            For,
            Return,
            Delete
        };

        Type type;

        enum class ValType
        {
            IfStatement,
            WhileLoop,
            Variable,
            Operand,
            FunctionCall,
            DataType
        };

        union
        {
            IfStatement* ifStatement;
            Variable* variable;
            Operand* operand;
            FunctionCall* functionCall;
            DataType* dataType;
            WhileLoop* whileLoop;
        }val;

        ValType valType;
        LocalScope* parentScope;
    };

    class IfStatement
    {
        public:
        bool Parse(Parser &parser, LocalScope &scope);
        bool Compile(CodeGenerator &cg, unsigned long long *ifLabel);
        bool hasNext;
        LocalScope* scope;

        private:
        Operand* expression;
        bool hasExpression;
    };

    class WhileLoop
    {
        public:
        bool Parse(Parser &parser, LocalScope &scope);
        bool Compile(CodeGenerator &cg);
        LocalScope* scope;

        private:
        Operand* expression;
        bool hasExpression;
    };

    class Namespace
    {
        public:
        //Namespace();
        bool Parse(Parser &parser, Namespace *scope);
        bool Compile(CodeGenerator &cg);
        bool ContainsFunction(std::string id);
        bool ContainsStructure(std::string id);
        std::string GetId(){return identifier;}
        std::vector<Structure*> GetStructs(){return types;}
        Variable* GetVariable(std::string id);
        Function* GetFunction(std::string id);
        Namespace* parent;

        private:
        bool hasParent;
        std::vector<Namespace*> namespaces;
        std::vector<Function*> functions;
        std::vector<Structure*> types;
        LocalScope* globals;
        std::string identifier;
        unsigned long long globalPos;
    };
}