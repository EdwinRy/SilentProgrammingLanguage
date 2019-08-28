#include "SParser.hpp"
#include <vector>
#pragma once


namespace Silent
{
    //enum class Bytecode
    //{
    //    Halt = 0,
    //    Goto,

    //    Call,
    //    Return,
    //    LoadDll,//
    //    LoadDllFunc,//
    //    FreeDll,//
    //    CallDllFunc,//

    //    Push1,
    //    Push2,
    //    Push4,
    //    Push8,
    //    PushX,

    //    Pop1,
    //    Pop2,
    //    Pop4,
    //    Pop8,
    //    PopX,

    //    Store1,
    //    Store2,
    //    Store4,
    //    Store8,
    //    StoreX,

    //    Load1,
    //    Load2,
    //    Load4,
    //    Load8,
    //    LoadX,

    //    StoreGlobal1,
    //    StoreGlobal2,
    //    StoreGlobal4,
    //    StoreGlobal8,
    //    StoreGlobalX,

    //    LoadGlobal1,
    //    LoadGlobal2,
    //    LoadGlobal4,
    //    LoadGlobal8,
    //    LoadGlobalX,

    //    Alloc1,
    //    Alloc2,
    //    Alloc4,
    //    Alloc8,
    //    AllocX,

    //    LoadPtr1,
    //    LoadPtr2,
    //    LoadPtr4,
    //    LoadPtr8,
    //    LoadPtrX,

    //    StorePtr1,
    //    StorePtr2,
    //    StorePtr4,
    //    StorePtr8,
    //    StorePtrX,
    //    Free,
    //    GetPtr,

    //    AddI1,
    //    AddI2,
    //    AddI4,
    //    AddI8,
    //    AddF4,
    //    AddF8,

    //    SubI1,
    //    SubI2,
    //    SubI4,
    //    SubI8,
    //    SubF4,
    //    SubF8,

    //    MulI1,
    //    MulI2,
    //    MulI4,
    //    MulI8,
    //    MulF4,
    //    MulF8,

    //    DivI1,
    //    DivI2,
    //    DivI4,
    //    DivI8,
    //    DivF4,
    //    DivF8,

    //    //Convert,
    //    SmallerThan,
    //    SmallerThanOrEqual,
    //    LargerThan,
    //    LargerThanOrEqual,
    //    Equal,
    //    NotEqual,

    //    If,
    //    IfNot,
    //    And,
    //    Or,
    //    Xor,
    //    Not,
    //    Label
    //};

    //std::string BytecodeToString(Bytecode code);

    //class Instruction
    //{
    //    class Parameter
    //    {
    //        public:
    //        friend class CodeGenerator;
    //        Parameter(std::string value);
    //        std::string val;
    //    };

    //    public:
    //    friend class CodeGenerator;
    //    Instruction(Bytecode code, const std::vector<Parameter> &params);
    //    Bytecode instruction;
    //    uint64_t size;
    //    std::vector<Parameter> params;

    //    void OutputInstruction();
    //};

    //class OperandOutput
    //{
    //    enum class Type
    //    {

    //    };
    //};

    //class CodeGenerator
    //{
    //    public:
    //    CodeGenerator
    //    (
    //        bool enableDebug = false, 
    //        bool enableWarnings = false, 
    //        bool enableErr = false
    //    );
    //    char* Compile(Program* ast);

    //    private:

    //    class ScopeInfo
    //    {
    //        friend class CodeGenerator;
    //        SymbolTable* symTable;
    //        uint64_t stackPtr;
    //        uint64_t ifPtr;
    //        
    //    }scopeInfo;

    //    class RuntimeInfo
    //    {
    //        friend class CodeGenerator;
    //        Subroutine* currentSub;
    //        Namespace* currentNamespace;
    //        Program* program;
    //    }runtimeInfo;

    //    std::vector<char> code;
    //    std::vector<Instruction> instructions;
    //    char* bytecodeOut = NULL;
    //    
    //    
    //    // Compile functions
    //    bool CompileProgram(Program* program);
    //    bool CompileNamespace(Namespace* scope);
    //    bool CompileSubroutine(Subroutine* function);
    //    bool CompileLocalStatement(LocalStatement localStatement);
    //    bool CompileExpression(Expression* expression);
    //    bool CompileVariableDecleration(Variable* varDec);
    //    bool CompileOperand(Operand* op);


    //    // Helper functions
    //    std::string GetScopeId(SymbolTable* symTable);
    //    std::string GenLabel(std::string val);
    //    void AddLabel(std::string val);
    //    void AddInstruction(Instruction instruction);
    //    Type* GetType(TypeName typeName);


    //    bool debugMsgEnabled;
    //    void DebugMsg(std::string msg);
    //    bool warningsEnabled;
    //    void WarningMsg(std::string msg);
    //    bool errorsEnabled;
    //    void ErrorMsg(std::string msg);
    //    uint64_t errorCount;
    //    uint64_t warningCount;

    //};

    //class AssemblyGenerator
    //{

    //};
}