#include "SCodeGen.hpp"
namespace Silent
{
    CodeGenerator::CodeGenerator
    (
        bool enableDebug, 
        bool enableWarnings, 
        bool enableErr
    )
    {
        debugMsgEnabled = enableDebug;
        warningsEnabled = enableWarnings;
        errorsEnabled = enableErr;
        errorCount = 0;
        warningCount = 0;
    }

    void CodeGenerator::DebugMsg(std::string msg)
    {
        if (debugMsgEnabled)
        {
            std::cout << msg << "\n";
        }
    }

    void CodeGenerator::WarningMsg(std::string msg)
    {
        warningCount++;
        if (warningsEnabled)
        {
            std::cout << "\033[1;33m" << "Compiler warning:" << "\033[0m\n"
                << "\033[4;33m" << msg << "\033[0m\n";
        }
    }

    void CodeGenerator::ErrorMsg(std::string msg)
    {
        errorCount++;
        if (errorsEnabled)
        {
            std::cout << "\033[1;31m" << "Compiler error:" << "\033[0m\n"
                << "\033[4;31m" << msg << "\033[0m\n";
        }

    }

    Instruction::Instruction(
        Bytecode code, const std::vector<Instruction::Parameter>& params)
    {
        instruction = code;
        this->params = params;
    }

    void Instruction::OutputInstruction()
    {
        std::cout << BytecodeToString(instruction) << " ";

        for (Parameter param : params)
            std::cout << param.val << " ";

        std::cout << "\n";
    }

    Instruction::Parameter::Parameter(std::string val)
    {
        this->val = val;
    }

    char* CodeGenerator::Compile(Program* ast)
    {
        DebugMsg("");
        scopeInfo.symTable = ast->table;
        scopeInfo.stackPtr = 0;
        scopeInfo.ifPtr = 0;
        if (CompileProgram(ast))
        {
            DebugMsg("Compilation successful");
        }
        else
        {
            DebugMsg("Compilation failed");
        }
        return NULL;
    }

    bool CodeGenerator::CompileProgram(Program* program)
    {
        DebugMsg("\033[36mCompiling program\033[0m");
        for (Function* f : program->functions)
            CompileFunction(f);

        for (Namespace* n : program->namespaces)
            CompileNamespace(n);

        DebugMsg("\033[32mFinished compiling program\033[0m");
        DebugMsg("Output:");

        for (Instruction i : instructions)
            i.OutputInstruction();
        return true;
    }

    bool CodeGenerator::CompileNamespace(Namespace* scope)
    {
        DebugMsg("\033[36mCompiling namespace: " + scope->id + "\033[0m");
        ScopeInfo oldInfo = scopeInfo;
        scopeInfo = ScopeInfo();
        scopeInfo.symTable = scope->symTable;
        AddLabel(scope->id);

        for (Function* f : scope->functions)
            CompileFunction(f);

        DebugMsg("\033[32mFinished compiling namespace: " + 
            scope->id + "\033[0m");
        scopeInfo = oldInfo;
        return true;
    }

    bool CodeGenerator::CompileFunction(Function* function)
    {
        DebugMsg("\033[36mCompiling function: " + function->id + "\033[0m");
        ScopeInfo oldInfo = scopeInfo;
        scopeInfo = ScopeInfo();
        scopeInfo.symTable = function->symTable;
        AddLabel(function->id);

        for (Function::LocalStatement ls : function->statements)
            CompileLocalStatement(ls);

        DebugMsg("\033[32mFinished compiling function: " + 
            function->id + "\033[0m");
        scopeInfo = oldInfo;
        return true;
    }

    bool CodeGenerator::CompileLocalStatement(Function::LocalStatement ls)
    {
        DebugMsg("Compiling a local statement");
        switch (ls.type)
        {
            case Function::LocalStatement::Type::Expression: 
                CompileExpression(ls.expression);
            break;

            case Function::LocalStatement::Type::VariableDeclaration: 
                CompileVariableDecleration(ls.varDec);
            break;
            
            default:
                ErrorMsg("Invalid statement in local scope");
                return false;
            break;
        }
        DebugMsg("Finished compiling a local statement");
        return true;
    }

    bool CodeGenerator::CompileExpression(Expression* expression)
    {
        DebugMsg("Compiling expression");
        CompileOperand(expression->op);
        DebugMsg("Finished compiling expression");
        return true;
    }

    bool CodeGenerator::CompileVariableDecleration(VariableDeclaration* varDec)
    {
        DebugMsg("Compiling declaration of variable: " + varDec->id);
        DebugMsg("Finished compiling declaration of variable: " + varDec->id);
        return true;
    }

    bool CodeGenerator::CompileOperand(Operand* op)
    {

        switch (op->type)
        {
            case Operand::Type::Add:
            {
                if (op->left != NULL)
                    CompileOperand(op->left);

                if(op->right != NULL)
                    CompileOperand(op->right);


            }
            break;

            case Operand::Type::Sub:
            {

            }
            break;

            case Operand::Type::Mul:
            {

            }
            break;

            case Operand::Type::Div:
            {

            }
            break;


            default:
            break;
        }
        return true;
    }

    std::string CodeGenerator::GetScopeId(SymbolTable* symTable)
    {
        switch (symTable->self.nodeType)
        {
            case TableNode::Type::Namespace:
            {
                Namespace* scope = (Namespace*)symTable->self.GetNode();
                return scope->id;
            }
            break;

            case TableNode::Type::Function:
            {
                Function* func = (Function*)symTable->self.GetNode();
                return func->id;
            }
            break;
        }
        return "";
    }

    std::string CodeGenerator::GenLabel(std::string val)
    {
        std::vector<std::string> scopeIDs;
        scopeIDs.push_back(val);

        SymbolTable* scope = scopeInfo.symTable->GetParent();
     
        while (scope != NULL &&
            scope->self.nodeType != TableNode::Type::Program)
        {
            std::string label = GetScopeId(scope);
            scopeIDs.push_back(label);
            scope = scope->GetParent();
        }

        std::string label = "";

        for (std::string scopeID : scopeIDs)
            label = scopeID + ":" + label;

        return label;
    }

    void CodeGenerator::AddLabel(std::string val)
    {
        std::string label = GenLabel(val);
        instructions.push_back(
            Instruction(Bytecode::Label,{Instruction::Parameter(label)})
        );

    }

    void CodeGenerator::AddInstruction(Instruction instruction)
    {
        instructions.push_back(instruction);
    }

    std::string BytecodeToString(Bytecode code)
    {
        switch (code)
        {
            case Bytecode::Halt: return "Halt"; break;
            case Bytecode::Goto: return "Goto"; break;
            case Bytecode::Call: return "Call"; break;
            case Bytecode::Return: return "Return"; break;

            case Bytecode::LoadDll: return "LoadDll"; break;
            case Bytecode::LoadDllFunc: return "LoadDllFunc"; break;
            case Bytecode::FreeDll: return "FreeDll"; break;
            case Bytecode::CallDllFunc: return "CallDllFunc"; break;

            case Bytecode::Push1: return "Push1"; break;
            case Bytecode::Push2: return "Push2"; break;
            case Bytecode::Push4: return "Push4"; break;
            case Bytecode::Push8: return "Push8"; break;
            case Bytecode::PushX: return "PushX"; break;

            case Bytecode::Pop1: return "Pop1"; break;
            case Bytecode::Pop2: return "Pop2"; break;
            case Bytecode::Pop4: return "Pop4"; break;
            case Bytecode::Pop8: return "Pop8"; break;
            case Bytecode::PopX: return "PopX"; break;

            case Bytecode::Store1: return "Store1"; break;
            case Bytecode::Store2: return "Store2"; break;
            case Bytecode::Store4: return "Store4"; break;
            case Bytecode::Store8: return "Store8"; break;
            case Bytecode::StoreX: return "StoreX"; break;

            case Bytecode::Load1: return "Load1"; break;
            case Bytecode::Load2: return "Load2"; break;
            case Bytecode::Load4: return "Load4"; break;
            case Bytecode::Load8: return "Load8"; break;
            case Bytecode::LoadX: return "LoadX"; break;

            case Bytecode::StoreGlobal1: return "StoreGlobal1"; break;
            case Bytecode::StoreGlobal2: return "StoreGlobal2"; break;
            case Bytecode::StoreGlobal4: return "StoreGlobal4"; break;
            case Bytecode::StoreGlobal8: return "StoreGlobal8"; break;
            case Bytecode::StoreGlobalX: return "StoreGlobalX"; break;

            case Bytecode::LoadGlobal1: return "LoadGlobal1"; break;
            case Bytecode::LoadGlobal2: return "LoadGlobal2"; break;
            case Bytecode::LoadGlobal4: return "LoadGlobal4"; break;
            case Bytecode::LoadGlobal8: return "LoadGlobal8"; break;
            case Bytecode::LoadGlobalX: return "LoadGlobalX"; break;

            case Bytecode::Alloc1: return "Alloc1"; break;
            case Bytecode::Alloc2: return "Alloc2"; break;
            case Bytecode::Alloc4: return "Alloc4"; break;
            case Bytecode::Alloc8: return "Alloc8"; break;
            case Bytecode::AllocX: return "AllocX"; break;
 
            case Bytecode::LoadPtr1: return "LoadPtr1"; break;
            case Bytecode::LoadPtr2: return "LoadPtr2"; break;
            case Bytecode::LoadPtr4: return "LoadPtr4"; break;
            case Bytecode::LoadPtr8: return "LoadPtr8"; break;
            case Bytecode::LoadPtrX: return "LoadPtrX"; break;

            case Bytecode::StorePtr1: return "StorePtr1"; break;
            case Bytecode::StorePtr2: return "StorePtr2"; break;
            case Bytecode::StorePtr4: return "StorePtr4"; break;
            case Bytecode::StorePtr8: return "StorePtr8"; break;
            case Bytecode::StorePtrX: return "StorePtrX"; break;
            case Bytecode::Free: return "Free"; break;
            case Bytecode::GetPtr: return "GetPtr"; break;

            case Bytecode::AddI1: return "AddI1"; break;
            case Bytecode::AddI2: return "AddI2"; break;
            case Bytecode::AddI4: return "AddI4"; break;
            case Bytecode::AddI8: return "AddI8"; break;
            case Bytecode::AddF4: return "AddF4"; break;
            case Bytecode::AddF8: return "AddF8"; break;

            case Bytecode::SubI1: return "SubI1"; break;
            case Bytecode::SubI2: return "SubI2"; break;
            case Bytecode::SubI4: return "SubI4"; break;
            case Bytecode::SubI8: return "SubI8"; break;
            case Bytecode::SubF4: return "SubF4"; break;
            case Bytecode::SubF8: return "SubF8"; break;

            case Bytecode::MulI1: return "MulI1"; break;
            case Bytecode::MulI2: return "MulI2"; break;
            case Bytecode::MulI4: return "MulI4"; break;
            case Bytecode::MulI8: return "MulI8"; break;
            case Bytecode::MulF4: return "MulF4"; break;
            case Bytecode::MulF8: return "MulF8"; break;

            case Bytecode::DivI1: return "DivI1"; break;
            case Bytecode::DivI2: return "DivI2"; break;
            case Bytecode::DivI4: return "DivI4"; break;
            case Bytecode::DivI8: return "DivI8"; break;
            case Bytecode::DivF4: return "DivF4"; break;
            case Bytecode::DivF8: return "DivF8"; break;

            case Bytecode::SmallerThan: return "SmallerThan"; break;
            case Bytecode::SmallerThanOrEqual: return "SmallerThanOrEqual"; break;
            case Bytecode::LargerThan: return "LargerThan"; break;
            case Bytecode::LargerThanOrEqual: return "LargerThanOrEqual"; break;
            case Bytecode::Equal: return "Equal"; break;
            case Bytecode::NotEqual: return "NotEqual"; break;

            case Bytecode::If: return "If"; break;
            case Bytecode::IfNot: return "IfNot"; break;
            case Bytecode::And: return "And"; break;
            case Bytecode::Or: return "Or"; break;
            case Bytecode::Xor: return "Xor"; break;
            case Bytecode::Not: return "Not"; break;
            case Bytecode::Label: return "Label"; break;

            default: return "Unknown"; break;
        }
    }
}