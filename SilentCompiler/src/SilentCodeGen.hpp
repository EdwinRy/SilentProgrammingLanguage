#include "SilentParser.hpp"
#include <vector>
#include <unordered_map>
#pragma once
namespace Silent
{
    enum class Opcodes
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
        PushX,

        Pop1,
        Pop2,
        Pop4,
        Pop8,
        PopX,

        Store1,
        Store2,
        Store4,
        Store8,
        StoreX,

        Load1,
        Load2,
        Load4,
        Load8,
        LoadX,

        StoreGlobal1,
        StoreGlobal2,
        StoreGlobal4,
        StoreGlobal8,
        StoreGlobalX,

        LoadGlobal1,
        LoadGlobal2,
        LoadGlobal4,
        LoadGlobal8,
        LoadGlobalX,

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

    class Bytecode
    {
        public:
        char* GetCode(uint64* size);
        std::vector<char>* GetPtrToCode(){return &code;}
        uint64 GetCodePointer(){return code.size();}
        char* GetBytecodeBuffer(){return code.data();}

        void AddPush(Structures::DataType dt, std::string val);
        template<typename T>
        void AddVal(T val);

        //To bytecode primitive datatype size
        //parameters: p - primitive, base - base instruction
        Opcodes ToBytecodeSize(Structures::Primitives p, 
            Opcodes base);
        //To bytecode expression based on primitive type
        Opcodes ToBytecodeExp(Structures::Primitives p, 
            Opcodes base);


        private:
        void AddData(Structures::DataType dt, std::string val);
        std::vector<char> code;
    };

    class CodeGenerator
    {
        public:
        void GenerateBytecode(Parser &parser);
        char* GetOutput(uint64* size){return code.GetCode(size);}
        void PushNamespace(Structures::Namespace* scope)
            {namespaces.push_back(scope);}
        void PopNamespace(){namespaces.pop_back();}
        std::string GetCurrentLocation();
        void AddFuncSym(Structures::Function* func);
        uint64 GetFuncPtr(Structures::Function* func)
            {return funcSymTable[func];}
        
        Bytecode code;

        Structures::DataType currentType;
        

        private:
        std::vector<Structures::Namespace*> namespaces;
        std::unordered_map<Structures::Function*, uint64> funcSymTable;
        

    };
}