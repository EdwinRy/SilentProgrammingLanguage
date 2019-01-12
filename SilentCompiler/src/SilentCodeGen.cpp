#include "SilentCodeGen.hpp"
#include "SilentParser.hpp"
#include "SilentStructures.hpp"
#include <iostream>
#include <cstring>

#define DEBUG_ENABLED 1
#define ERROR(args...) printf(args);
#if DEBUG_ENABLED
#define DEBUG(args...) printf(args);
#else
#define DEBUG(args...) 
#endif

typedef unsigned long long uint64;
namespace Silent::Structures
{
    bool Operand::Compile(CodeGenerator &gc)
    {
        DEBUG("Compiling operand\n");
        switch(this->operandType)
        {
            case OperandType::Assign:
            {
                DEBUG("ASSIGNMENT\n");
                DataType oldType = gc.currentType;
                //Variable* var = this->left->value.variable;
                Variable var;
                if(this->left->value.nodeType == Node::Type::Members)
                {
                    var = *this->left->value.members->back();
                    uint64 offset = 0;
                    for(Variable* tempVar : *this->left->value.members)
                        offset += tempVar->GetLocalPos();

                    var.SetLocalPos(offset);
                    DEBUG("MEMBER OFFSET %llu\n", offset);
                }
                else var = *this->left->value.variable;


                gc.currentType = var.GetType();
                this->right->Compile(gc);
                if(var.isReference)
                {
                    if(var.GetType().dataType == DataType::Type::Primitive)
                    {
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var.GetType().primitive,Opcodes::Load1));
                        gc.code.AddVal<uint64>(var.GetLocalPos());
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var.GetType().primitive,Opcodes::StorePtr1));
                        gc.code.AddVal<uint64>(0ll);
                    }
                }
                else
                {
                    if(var.GetType().dataType == DataType::Type::Primitive)
                    {
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var.GetType().primitive,Opcodes::Store1));
                        gc.code.AddVal<uint64>(var.GetLocalPos());
                    }
                }
                gc.currentType = oldType;
            }
            break;

            case OperandType::FunctionCall:
            {
                DEBUG("FUNCTION CALL\n");

                FunctionCall* funcCall = this->value.functionCall;
                Function* func = funcCall->function;

                for(uint64 i = 0; i < func->GetParameterCount(); i++)
                {
                    Operand* arg = funcCall->arguments[i];
                    if(func->GetScope()->GetVariables()->at(i)->isReference)
                    {
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            arg->value.variable->GetType().primitive, 
                            Opcodes::Load1
                        ));
                        gc.code.AddVal<uint64>(
                            arg->value.variable->GetLocalPos());
                    }
                    else arg->Compile(gc);
                }
                gc.code.AddVal<char>((char)Opcodes::Call);
                gc.code.AddVal<uint64>(gc.GetFuncPtr(func));
                gc.code.AddVal<uint64>(func->GetParameterSize());
            }
            break;

            case OperandType::Add:
                DEBUG("ADDITION\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)gc.code.ToBytecodeExp(
                    gc.currentType.primitive,Opcodes::AddI1));
            break;

            case OperandType::Subtract:
                DEBUG("SUBTRACTION\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)gc.code.ToBytecodeExp(
                    gc.currentType.primitive,Opcodes::SubI1));
            break;

            case OperandType::Multiply:
                DEBUG("MULTIPLICATION\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)gc.code.ToBytecodeExp(
                    gc.currentType.primitive,Opcodes::MulI1));
            break;

            case OperandType::Divide:
                DEBUG("DIVISION\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)gc.code.ToBytecodeExp(
                    gc.currentType.primitive,Opcodes::DivI1));
            break;

            case OperandType::Equal:
                DEBUG("EQUAL TO\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::Equal);
                gc.code.AddVal<char>(
                    (char)gc.currentType.primitive);
            break;

            case OperandType::NotEqual:
                DEBUG("NOT EQUAL TO\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::NotEqual);
                gc.code.AddVal<char>(
                    (char)gc.currentType.primitive);
            break;

            case OperandType::Larger:
                DEBUG("LARGER THAN\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::LargerThan);
                gc.code.AddVal<char>(
                    (char)gc.currentType.primitive);
            break;

            case OperandType::LargerOrEqual:
                DEBUG("LARGER THAN OR EQUAL TO\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::LargerThanOrEqual);
                gc.code.AddVal<char>(
                    (char)gc.currentType.primitive);
            break;

            case OperandType::Smaller:
                DEBUG("SMALLER THAN\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::SmallerThan);
                gc.code.AddVal<char>(
                    (char)gc.currentType.primitive);
            break;

            case OperandType::SmallerOrEqual:
                DEBUG("SMALLER THAN OR EQUAL TO\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::SmallerThanOrEqual);
                gc.code.AddVal<char>(
                    (char)gc.currentType.primitive);
            break;

            case OperandType::And:
                DEBUG("AND\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::And);
            break;

            case OperandType::Or:
                DEBUG("OR\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::Or);
            break;

            case OperandType::Xor:
                DEBUG("XOR\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::Xor);
            break;

            case OperandType::Not:
                DEBUG("NOT\n");
                this->left->Compile(gc);
                this->right->Compile(gc);
                gc.code.AddVal<char>((char)Opcodes::Not);
            break;

            case OperandType::Number:
            {
                DEBUG("NUMBER\n");
                if(gc.currentType.primitive == Primitives::null)
                    gc.currentType.primitive = Primitives::int64;
                gc.code.AddPush(gc.currentType, this->value.value->data);
            }
            break;

            case OperandType::Members:
            {
                DEBUG("STRUCT\n");
                DataType oldType = gc.currentType;
                gc.currentType = this->value.members->back()->GetType();

                //Variable* lastMember = this->value.members->back();
                //this->value.members->pop_back();

                uint64 offset = 0;
                for(Variable* var : *this->value.members)
                    offset += var->GetLocalPos();

                DEBUG("Member offset -> %i\n", offset);

                Variable* var = this->value.members->back();

                if(var->isReference)
                {
                    // if(var->GetType().dataType == DataType::Type::Primitive)
                    // {
                    //     gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                    //         var->GetType().primitive,
                    //         Opcodes::Load1
                    //     ));
                    //     gc.code.AddVal<uint64>(var->GetLocalPos());
                    //     gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                    //         var->GetType().primitive,
                    //         Opcodes::StorePtr1
                    //     ));
                    //     gc.code.AddVal<uint64>(0ll);
                    // }
                    // else
                    // {
                    //     //TODO: add init structure reference variables
                    // }
                }
                else
                {
                    if(var->GetType().dataType == DataType::Type::Primitive)
                    {
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var->GetType().primitive,
                            Opcodes::Load1
                        ));
                        gc.code.AddVal<uint64>(offset);
                    }
                    else
                    {
                        //TODO: add init structure variables
                    }
                }

                gc.currentType = oldType;
            }
            break;

            case OperandType::Variable:
            {
                DEBUG("VARIABLE\n");
                DataType oldType = gc.currentType;
                Variable* var = this->value.variable;
                gc.currentType = var->GetType();
                if(var->isReference)
                {
                    if(var->GetType().dataType == DataType::Type::Primitive)
                    {
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var->GetType().primitive,
                            Opcodes::Load1
                        ));
                        gc.code.AddVal<uint64>(var->GetLocalPos());
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var->GetType().primitive,
                            Opcodes::StorePtr1
                        ));
                        gc.code.AddVal<uint64>(0ll);
                    }
                    else
                    {
                        //TODO: add init structure reference variables
                    }
                }
                else
                {
                    if(var->GetType().dataType == DataType::Type::Primitive)
                    {
                        gc.code.AddVal<char>((char)gc.code.ToBytecodeSize(
                            var->GetType().primitive,
                            Opcodes::Load1
                        ));
                        gc.code.AddVal<uint64>(var->GetLocalPos());
                    }
                    else
                    {
                        //TODO: add init structure variables
                    }
                }
                gc.currentType = oldType;
            }
            break;

            default: break;
        }
        DEBUG("Done compiling operand\n");
        return true;
    }

    bool Statement::Compile(CodeGenerator &gc)
    {
        DEBUG("Compiling statement\n");
        switch(this->type)
        {
            case Statement::Type::VarInit:
                if(this->val.variable->isReference)
                {
                    switch(this->val.variable->GetSize())
                    {
                        case 1: 
                            gc.code.AddVal<char>((char)Opcodes::Alloc1);
                        break;
                        case 2: 
                            gc.code.AddVal<char>((char)Opcodes::Alloc2);
                        break;
                        case 4: 
                            gc.code.AddVal<char>((char)Opcodes::Alloc4);
                        break;
                        case 8: 
                            gc.code.AddVal<char>((char)Opcodes::Alloc8);
                        break;
                        default:
                        {
                            gc.code.AddVal<char>((char)Opcodes::AllocX);
                            gc.code.AddVal<uint64>(
                                this->val.variable->GetSize());
                        }
                        break;
                    }
                }
                else gc.code.AddPush(this->val.variable->GetType(),"0");
            break;

            case Statement::Type::Delete:
                gc.code.AddVal<char>((char)Opcodes::Free);
                gc.code.AddVal<uint64>(val.variable->GetLocalPos());
            break;

            case Statement::Type::Expression:
                this->val.operand->Compile(gc);
            break;

            case Statement::Type::Return:
                gc.code.AddVal<char>((char)Opcodes::Return);
                gc.code.AddVal<uint64>(gc.currentType.size);

            break;

            default: break;
        }
        DEBUG("Done compiling statement\n")
        return true;
    }

    bool IfStatement::Compile(CodeGenerator &gc, uint64 *ifLabel)
    {
        DEBUG("Compiling if statement\n");

        //Compile if statement's expression if it has one
        if(this->hasExpression)
        {
            this->expression->Compile(gc);
            //Add if not as it should branch if the condition is not true
            gc.code.AddVal<char>((char)Opcodes::IfNot);
            uint64 ifPtrIndex = gc.code.GetCodePointer();
            gc.code.AddVal<uint64>(0ll);
            this->scope->Compile(gc);
            //If it has an else then add goto to the end
            if(this->hasNext)
            {
                gc.code.AddVal<char>((char)Opcodes::Goto);
                *ifLabel = gc.code.GetCodePointer();
                gc.code.AddVal<uint64>(0ll);
            }
            else *ifLabel = 0;

            uint64 ifStatementEndPtr = gc.code.GetCodePointer();
            memcpy(gc.code.GetBytecodeBuffer()+ifPtrIndex,&ifStatementEndPtr,8);
        }
        else this->scope->Compile(gc);

        DEBUG("Done compiling if statement\n");
        return true;
    }

    bool WhileLoop::Compile(CodeGenerator &gc)
    {
        DEBUG("Compiling while loop\n");

        if(this->hasExpression)
        {
            uint64 top = gc.code.GetCodePointer();
            this->expression->Compile(gc);
            gc.code.AddVal<char>((char)Opcodes::IfNot);
            uint64 whilePtrIndex = gc.code.GetCodePointer();
            gc.code.AddVal<uint64>(0ll);
            this->scope->Compile(gc);

            gc.code.AddVal<char>((char)Opcodes::Goto);
            gc.code.AddVal<uint64>(top);
            uint64 whileEndPtr = gc.code.GetCodePointer();
            memcpy(gc.code.GetBytecodeBuffer()+whilePtrIndex,&whileEndPtr,8);
        }
        else
        {
            uint64 top = gc.code.GetCodePointer();
            this->scope->Compile(gc);
            gc.code.AddVal<char>((char)Opcodes::Goto);
            gc.code.AddVal<uint64>(top);
        }

        DEBUG("Done compiling while loop\n");
        return true;
    }

    bool LocalScope::Compile(CodeGenerator &gc)
    {
        DEBUG("Compiling local scope\n");

        //Program counter at the point if statements end
        std::vector<uint64> ifEndings;

        //Compile statements
        for(Statement* statement : this->statements)
        {
            switch(statement->type)
            {
                case Statement::Type::If:
                {
                    uint64 ifLabel;
                    statement->val.ifStatement->Compile(gc, &ifLabel);
                    if(ifLabel > 0) ifEndings.push_back(ifLabel);
                    //Resolve if's goto statements
                    if(statement->val.ifStatement->hasNext == false)
                    {
                        if(ifEndings.size() > 0)
                        {
                            uint64 codePtr = gc.code.GetCodePointer();
                            for(uint64 i : ifEndings)
                            {
                                memcpy(gc.code.GetBytecodeBuffer()+i,
                                    &codePtr,8);
                            }
                        }
                    }
                }
                case Statement::Type::While:
                    statement->val.whileLoop->Compile(gc);
                break;
                default: statement->Compile(gc); break;
            }
        }

        DEBUG("Done compiling local scope\n");
        return true;
    }

    bool Function::Compile(CodeGenerator &gc)
    {
        std::string symbol = gc.GetCurrentLocation()+identifier;
        DEBUG("Compiling function: %s\n", symbol.data());

        gc.AddFuncSym(this);

        //Insert main function's address after goto at the beginning
        if(this->identifier == "main")
        {
            char* codeBuff = gc.code.GetBytecodeBuffer();
            uint64 funcPtr = gc.code.GetCodePointer();
            memcpy(codeBuff + 1, &funcPtr, 8);
        }

        gc.currentType = this->returnType;
        this->scope->Compile(gc);

        //Add function to symbols table
        DEBUG("Done compiling function: %s\n", symbol.data());
        return true;
    }

    bool Namespace::Compile(CodeGenerator &gc)
    {
        std::string scope = gc.GetCurrentLocation() + this->identifier;
        DEBUG("Compiling namespace: %s\n", scope.data());
        gc.PushNamespace(this);

        for(Namespace* scope : this->namespaces)
            scope->Compile(gc);

        for(Function* func : this->functions)
            func->Compile(gc);

        gc.PopNamespace();
        DEBUG("Done compiling namespace: %s\n", scope.data());
        return true;
    }
}

using namespace Silent::Structures;
namespace Silent
{
    char* Bytecode::GetCode(uint64* size)
    {
        std::cout << "Size:" << code.size() << "\n";
        *size = code.size();
        //*returnCode = (char*)malloc(code.size()+1);
        //*returnCode[code.size()] = '\0';
        char* temp = (char*)malloc(code.size()+1);
        memcpy(temp, code.data(), code.size());
        temp[code.size()] = '\0';

        return temp;
        
    }

    void Bytecode::AddPush(DataType dt, std::string val)
    {
        //var->GetType().dataType == DataType::Type::Primitive
        if(dt.dataType == DataType::Type::Primitive)
        {
            AddVal<char>(
                (char)ToBytecodeSize(dt.primitive, 
                    Opcodes::Push1));
            AddData(dt, val);
        }
        else
        {
            char numVal = (char)std::stoi(val,nullptr,10);
            for(uint64 i = code.size(); i<dt.size; i++) code.push_back(numVal);
        }
    }

    template <typename T>
    void Bytecode::AddVal(T val)
    {
        code.resize(code.size() + sizeof(T));
        memcpy(code.data() + code.size() - sizeof(T), &val, sizeof(T));
    }

    Opcodes Bytecode::ToBytecodeSize(Primitives p, Opcodes base)
    {
        char index;
        switch(p)
        {
            case Primitives::int8:
            case Primitives::uint8:
                index = 0;
            break;
            case Primitives::int16:
            case Primitives::uint16:
                index = 1;
            break;
            case Primitives::int32:
            case Primitives::uint32:
            case Primitives::float32:
                index = 2;
            break;
            case Primitives::int64:
            case Primitives::uint64:
            case Primitives::float64:
                index = 3;
            break;
            default: index = 2; break;
        }
        return (Opcodes)((char)base + index);
    }

    Opcodes Bytecode::ToBytecodeExp(Primitives p, Opcodes base)
    {
        char index;
        switch(p)
        {
            case Primitives::int8:
            case Primitives::uint8:
                index = 0;
            break;
            case Primitives::int16:
            case Primitives::uint16:
                index = 1;
            break;
            case Primitives::int32:
            case Primitives::uint32:
                index = 2;
            break;
            case Primitives::int64:
            case Primitives::uint64:
                index = 3;
            break;
            case Primitives::float32:
                index = 4;
            break;
            case Primitives::float64:
                index = 5;
            break;
            default: index = 2; break;
        }
        return (Opcodes)((char)base + index);
    }

    void Bytecode::AddData(DataType dt, std::string val)
    {
        switch(dt.primitive)
        {
            case Primitives::int8:
            AddVal<char>(std::stoi(val,nullptr,10));
            break;
            case Primitives::uint8:
            AddVal<unsigned char>(std::stoi(val,nullptr,10));
            break;
            case Primitives::int16:
            AddVal<short>(std::stoi(val,nullptr,10));
            break;
            case Primitives::uint16:
            AddVal<unsigned short>(std::stoi(val,nullptr,10));
            break;
            case Primitives::int32:
            AddVal<int>(std::stoi(val,nullptr,10));
            break;
            case Primitives::uint32:
            AddVal<unsigned int>(std::stoi(val,nullptr,10));
            break;
            case Primitives::int64:
            AddVal<long long>(std::stoi(val,nullptr,10));
            break;
            case Primitives::uint64:
            AddVal<unsigned long long>(std::stoi(val,nullptr,10));
            break;
            case Primitives::float32:
            AddVal<float>(std::stof(val,nullptr));
            break;
            case Primitives::float64:
            AddVal<long double>(std::stof(val,nullptr));
            break;
            default: break;
        }
    }

    void CodeGenerator::GenerateBytecode(Parser &parser)
    {
        //Add goto main at the start of the program
        code.AddVal<char>((char)Opcodes::Goto);
        code.AddVal<uint64>((uint64)0ll);
        //Add halt when main returns
        code.AddVal<char>((char)Opcodes::Halt);
        //Compile global namespace
        if(parser.GetGlobalNamespace()->Compile(*this) == true)
        {
            DEBUG("Bytecode generated successfully\n");
        }
        code.AddVal<char>((char)Opcodes::Halt);
    }

    std::string CodeGenerator::GetCurrentLocation()
    {
        std::string location;
        for(Namespace* scope : this->namespaces)
            location += scope->GetId() + "::";
        return location;
    }

    void CodeGenerator::AddFuncSym(Function* func)
    {
        funcSymTable[func] = code.GetCodePointer();
    }
}