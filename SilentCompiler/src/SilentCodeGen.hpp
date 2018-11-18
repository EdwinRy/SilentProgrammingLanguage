#pragma once
#include "SilentParser.hpp"
#include <vector>
#include <unordered_map>
namespace Silent
{
    enum class SilentBytecode
    {
        Halt = 0,
        Goto,

        Call,
        Return,
        LoadDll,//
        LoadDllFunc,//
        FreeDll,//
        CallDllFunc,//

        Push1,
        Push2,
        Push4,
        Push8,

        Pop1,
        Pop2,
        Pop4,
        Pop8,

        Store1,
        Store2,
        Store4,
        Store8,

        Load1,
        Load2,
        Load4,
        Load8,

        StoreGlobal1,
        StoreGlobal2,
        StoreGlobal4,
        StoreGlobal8,

        LoadGlobal1,
        LoadGlobal2,
        LoadGlobal4,
        LoadGlobal8,

        Alloc1,
        Alloc2,
        Alloc4,
        Alloc8,
        AllocX,

        LoadPtr1,
        LoadPtr2,
        LoadPtr4,
        LoadPtr8,
        LoadPtrX,

        StorePtr1,
        StorePtr2,
        StorePtr4,
        StorePtr8,
        StorePtrX,
        Free,
        GetPtr,

        AddI1,
        AddI2,
        AddI4,
        AddI8,
        AddF4,
        AddF8,

        SubI1,
        SubI2,
        SubI4,
        SubI8,
        SubF4,
        SubF8,

        MulI1,
        MulI2,
        MulI4,
        MulI8,
        MulF4,
        MulF8,

        DivI1,
        DivI2,
        DivI4,
        DivI8,
        DivF4,
        DivF8,

        //Convert,
        SmallerThan,
        SmallerThanOrEqual,
        LargerThan,
        LargerThanOrEqual,
        Equal,
        NotEqual,

        If,
        IfNot,
        And,
        Or,
        Xor,
        Not
    };

    class SilentCode
    {
        public:
        void AddPush(SilentDataType dt, std::string val);
        void AddLoad(SilentDataType dt, uint64 localPos);
        void AddStore(SilentDataType dt, uint64 localPos);
        template<typename T>
        void AddNumber(T val);
        std::string GetCode();
        std::vector<char>* GetPtrToCode();
        uint64 GetCodePointer();
        SilentBytecode ToBytecodeSize(SilentPrimitives p, SilentBytecode base);
        SilentBytecode ToBytecodeExp(SilentPrimitives p, SilentBytecode base);

        private:
        void AddData(SilentDataType dt, std::string val);
        //SilentVMType ToVMType(SilentPrimitives p);
        std::vector<char> code;
    };

    class SilentCodeGenerator
    {
        public:
        void Compile(SilentParser* parser);
        std::string GetOutput();

        private:
        std::string GenScopeName(std::string id);
        void CompileExpression(SilentOperand &expression);
        void CompileStatement(SilentStatement &statement);
        void CompileLocalScope(SilentLocalScope &scope);
        void CompileFunction(SilentFunction &func);
        void CompileNamespace(SilentNamespace &scope);
        
        std::vector<SilentNamespace*> namespaces;
        std::unordered_map<SilentFunction*, uint64> symTable;
        SilentCode code;
        //SilentPrimitives currentPrimitive;
        SilentDataType currentDataType;
    };
}