#include "SilentCodeGen.hpp"
#include "SilentParser.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
using namespace Silent;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#define DEBUG 1

namespace Silent
{
    SilentBytecode SilentCode::ToBytecodeSize(
        SilentPrimitives p, SilentBytecode base)
    {
        char index;
        switch(p)
        {
            case SilentPrimitives::int8:
            case SilentPrimitives::uint8: index = 0; break;
            case SilentPrimitives::int16:
            case SilentPrimitives::uint16: index = 1; break;
            case SilentPrimitives::int32:
            case SilentPrimitives::uint32: 
            case SilentPrimitives::float32: index = 2; break;
            case SilentPrimitives::int64:
            case SilentPrimitives::uint64: 
            case SilentPrimitives::float64: index = 3; break;
            default: index = 2; break;
        }
        return (SilentBytecode)((char)base+index);
    }

    SilentBytecode SilentCode::ToBytecodeExp(
        SilentPrimitives p, SilentBytecode base)
    {
        char index;
        switch(p)
        {
            case SilentPrimitives::int8:
            case SilentPrimitives::uint8: index = 0; break;
            case SilentPrimitives::int16:
            case SilentPrimitives::uint16: index = 1; break;
            case SilentPrimitives::int32:
            case SilentPrimitives::uint32: index = 2; break;
            case SilentPrimitives::int64:
            case SilentPrimitives::uint64: index = 3; break;
            case SilentPrimitives::float32: index = 4; break;
            case SilentPrimitives::float64: index = 5; break;
            default: index = 2; break;
        }
        return (SilentBytecode)((char)base+index);
    }

    void SilentCode::AddData(SilentDataType dt, std::string val)
    {
        switch(dt.primitive)
        {
            case SilentPrimitives::int8:
            AddNumber<char>(std::stoi(val,nullptr,10));break;
            case SilentPrimitives::uint8:
            AddNumber<unsigned char>(std::stoi(val,nullptr,10));break;
            case SilentPrimitives::int16:
            AddNumber<short>(std::stoi(val,nullptr,10));break;
            case SilentPrimitives::uint16:
            AddNumber<unsigned short>(std::stoi(val,nullptr,10));break;
            case SilentPrimitives::int32:
            AddNumber<int>(std::stol(val,nullptr,10));break;
            case SilentPrimitives::uint32:
            AddNumber<unsigned int>(std::stoul(val,nullptr,10));break;
            case SilentPrimitives::int64:
            AddNumber<long long>(std::stoll(val,nullptr,10));break;
            case SilentPrimitives::uint64:
            AddNumber<uint64>(std::stoull(val,nullptr,10));break;
            case SilentPrimitives::float32:
            AddNumber<float>(std::stof(val,nullptr));break;
            case SilentPrimitives::float64:
            AddNumber<long double>(std::stold(val,nullptr));break;
            default: break;
        }
    }

    void SilentCode::AddPush(SilentDataType dt, std::string val)
    {
        if(dt.isPrimitive)
        {
            // AddNumber<char>((char)ToBytecodePush(dt.primitive));
            AddNumber<char>(
                (char)ToBytecodeSize(dt.primitive,SilentBytecode::Push1));
            //AddNumber<char>((char)ToVMType(dt.primitive));
            AddData(dt, val);
        }
    }

    // void SilentCode::AddLoad(SilentDataType dt, uint64 localPos)
    // {
    //     if(dt.isPrimitive)
    //     {
    //         AddNumber<char>(
    //             (char)ToBytecodeSize(dt.primitive,SilentBytecode::Load1)
    //         );
    //         //AddNumber<char>((char)ToVMType(dt.primitive));
    //         AddNumber<uint64>(localPos);
    //     }
    //     // else
    //     // {
    //     //     AddNumber<char>(
    //     //         (char)ToBytecodeSize(dt.primitive,SilentBytecode::LoadX)
    //     //     );

    //     // }
    // }

    // void SilentCode::AddStore(SilentDataType dt, uint64 localPos)
    // {
    //     if(dt.isPrimitive)
    //     {
    //         AddNumber<char>(
    //             (char)ToBytecodeSize(dt.primitive,SilentBytecode::Store1)
    //         );
    //         AddNumber<uint64>(localPos);
    //     }
    // }

    template<typename T>
    void SilentCode::AddNumber(T val)
    {
        code.resize(code.size() + sizeof(T));
        memcpy(code.data() + code.size() - sizeof(T), &val, sizeof(T));
    }

    std::string SilentCode::GetCode()
    {
        std::string str(code.begin(), code.end());
        return str;
    }

    std::vector<char>* SilentCode::GetPtrToCode() { return &this->code; }

    uint64 SilentCode::GetCodePointer() { return this->code.size(); }


    void SilentCodeGenerator::Compile(SilentParser *parser)
    {
        code.AddNumber<char>((char)SilentBytecode::Goto);
        code.AddNumber<uint64>((uint64)0ll);
        CompileNamespace(*parser->GetGlobalNamespace());
        code.AddNumber<char>((char)SilentBytecode::Halt);
    }

    std::string SilentCodeGenerator::GetOutput() { return code.GetCode(); }

    std::string SilentCodeGenerator::GenScopeName(std::string id)
    {
        std::string name = "";
        for(SilentNamespace *scope : namespaces)
        {
            name += scope->name;
            name += "::";
        }
        name += id;
        return name;
    }

    void SilentCodeGenerator::CompileExpression(
        SilentOperand &expression
    )
    {
        #if DEBUG
        std::cout << "Compiling expression\n";
        #endif
        switch(expression.type)
        {
            case SilentOperandType::Assign:
            {
                printf("Assignment of type %i\n",
                    (int)expression.left->variable->type.primitive);
                SilentDataType oldType = currentDataType;
                currentDataType = expression.left->variable->type;

                SilentVariable* var = expression.left->variable;
                CompileExpression(*expression.right);
                if(var->isReference)
                {
                    if(var->type.isPrimitive)
                    {
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(var->type.primitive, 
                            SilentBytecode::Load1)
                        );
                        code.AddNumber<uint64>(var->localPos);
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(var->type.primitive, 
                            SilentBytecode::StorePtr1)
                        );
                        code.AddNumber<uint64>(0ll);
                    }
                }
                else
                {
                    if(var->type.isPrimitive)
                    {
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(var->type.primitive, 
                            SilentBytecode::Store1)
                        );
                        code.AddNumber<uint64>(var->localPos);
                    }
                }
                currentDataType = oldType;
            }
            break;

            case SilentOperandType::FunctionCall:
            {
                printf("Calling function %s\n",
                    expression.functionCall->function->name.data());
                //expression.functionCall

                //for(SilentOperand* arg : expression.functionCall->arguments)
                //    CompileExpression(*arg);

                //Compile arguments
                SilentFunction* calledFunc = expression.functionCall->function;
                for(uint64 i = 0; i < calledFunc->parameterCount; i++)
                {
                    SilentOperand* arg = expression.functionCall->arguments[i];
                    if(calledFunc->scope->variables[i]->isReference)
                    {
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(
                                arg->variable->type.primitive,
                                SilentBytecode::Load1)
                        );
                        code.AddNumber<uint64>(arg->variable->localPos);

                    }
                    else CompileExpression(*arg);
                }

                SilentFunction* calledFunction = 
                    expression.functionCall->function;
                code.AddNumber<char>((char)SilentBytecode::Call);
                code.AddNumber<uint64>(symTable[calledFunction]);
                //code.AddNumber<uint64>(calledFunction->parameterCount);
                code.AddNumber<uint64>(calledFunction->parameterSize);
            }
            break;

            case SilentOperandType::Add:
                printf("Addition\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                //AddNumber<char>(
                //    (char)ToBytecode(dt.primitive,SilentBytecode::Push1));
                code.AddNumber<char>(
                    (char)code.ToBytecodeExp(currentDataType.primitive, 
                    SilentBytecode::AddI1));
                //code.AddNumber<char>((char)SilentBytecode::Add);
                
            break;

            case SilentOperandType::Subtract:
                printf("Subtraction\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                //code.AddNumber<char>((char)SilentBytecode::Sub);
                code.AddNumber<char>(
                    (char)code.ToBytecodeExp(currentDataType.primitive, 
                    SilentBytecode::SubI1));
            break;

            case SilentOperandType::Multiply:
                printf("Multiplication\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                //code.AddNumber<char>((char)SilentBytecode::Mul);
                code.AddNumber<char>(
                    (char)code.ToBytecodeExp(currentDataType.primitive, 
                    SilentBytecode::MulI1));
            break;

            case SilentOperandType::Divide:
                printf("Division\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                //code.AddNumber<char>((char)SilentBytecode::Div);
                code.AddNumber<char>(
                    (char)code.ToBytecodeExp(currentDataType.primitive, 
                    SilentBytecode::MulI1));
            break;

            case SilentOperandType::Equal:
                printf("Equal\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::Equal);
                code.AddNumber<char>((char)currentDataType.primitive);
            break;

            case SilentOperandType::NotEqual:
                printf("Not Equal\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::NotEqual);
                code.AddNumber<char>((char)currentDataType.primitive);
            break;

            case SilentOperandType::Larger:
                printf("Larger than\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::LargerThan);
                code.AddNumber<char>((char)currentDataType.primitive);
            break;

            case SilentOperandType::LargerOrEqual:
                printf("Larger than or equal to\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::LargerThanOrEqual);
                code.AddNumber<char>((char)currentDataType.primitive);
            break;

            case SilentOperandType::Smaller:
                printf("Smaller than\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::SmallerThan);
                code.AddNumber<char>((char)currentDataType.primitive);
            break;

            case SilentOperandType::SmallerOrEqual:
                printf("Smaller than or equal to\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::SmallerThanOrEqual);
                code.AddNumber<char>((char)currentDataType.primitive);
            break;

            case SilentOperandType::And:
                printf("AND\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::And);
            break;

            case SilentOperandType::Or:
                printf("OR\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::Or);
            break;
            
            case SilentOperandType::Xor:
                printf("XOR\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::Xor);
            break;

            case SilentOperandType::Not:
                printf("NOT\n");
                CompileExpression(*expression.left);
                CompileExpression(*expression.right);
                code.AddNumber<char>((char)SilentBytecode::Not);
            break;

            case SilentOperandType::Number:
                //code.AddPush(currentType, expression.token->value);
                currentDataType = expression.value->dataType;
                code.AddPush(expression.value->dataType,expression.value->data);
            break;

            case SilentOperandType::Variable:
                if(expression.variable->isReference)
                {
                    //ToBytecodeSize
                    if(expression.variable->type.isPrimitive)
                    {
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(
                                expression.variable->type.primitive,
                                SilentBytecode::Load1)
                        );
                        code.AddNumber<uint64>(expression.variable->localPos);
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(
                                expression.variable->type.primitive,
                                SilentBytecode::LoadPtr1)
                        );
                        //AddNumber<char>((char)ToVMType(dt.primitive));
                        code.AddNumber<uint64>(0ll);
                    }
                    else
                    {

                    }
                }
                else
                {
                    // code.AddLoad(expression.variable->type,
                    //     expression.variable->localPos);
                    if(expression.variable->type.isPrimitive)
                    {
                        code.AddNumber<char>((char)
                            code.ToBytecodeSize(
                                expression.variable->type.primitive,
                                SilentBytecode::Load1)
                        );
                        //AddNumber<char>((char)ToVMType(dt.primitive));
                        code.AddNumber<uint64>(expression.variable->localPos);
                    }
                    else
                    {
                        // AddNumber<char>(
                        //     (char)ToBytecodeSize(dt.primitive,SilentBytecode::LoadX)
                        // );

                    }
                }
            break;

            default: break;
        }
        #if DEBUG
        std::cout << "Done compiling expression\n";
        #endif
    }

    void SilentCodeGenerator::CompileIfStatement(
        SilentStatement &statement, uint64 *ifNotLabel)
    {
        #if DEBUG
        std::cout << "Compiling if statement\n";
        std::cout << "Code PTR: " << code.GetCodePointer() << "\n";
        #endif
        //Compile if expression if it has one
        if(statement.ifStatement->hasExpression)
        {
            CompileExpression(*statement.ifStatement->expression);
            code.AddNumber<char>((char)SilentBytecode::IfNot);
            uint64 ifPtrIndex = code.GetCodePointer();
            code.AddNumber<uint64>(0ll);
            CompileLocalScope(*statement.ifStatement->scope);
            //Add goto to the end of the statement if it has a next condition
            if(statement.ifStatement->hasNext)
            {
                code.AddNumber<char>((char)SilentBytecode::Goto);
                #if DEBUG
                std::cout << "Added GOTO\n";
                std::cout << "Code PTR: " << code.GetCodePointer() << "\n";
                #endif
                *ifNotLabel = code.GetCodePointer();
                code.AddNumber<uint64>(0ll);
            }
            else ifNotLabel = 0;
            //Get current code position
            uint64 ifEndPtrIndex = code.GetCodePointer();
            /*
                Copy current position to a goto if the 
                condition of the if is false
            */
            memcpy(code.GetPtrToCode()->data() + ifPtrIndex, &ifEndPtrIndex, 8);
        }
        else CompileLocalScope(*statement.ifStatement->scope);
        #if DEBUG
        std::cout << "END of if\n";
        std::cout << "Code PTR: " << code.GetCodePointer() << "\n";
        #endif
    }

    void SilentCodeGenerator::CompileStatement(SilentStatement &statement)
    {
        #if DEBUG
        std::cout << "Compiling statement\n";
        #endif

        switch(statement.type)
        {
            case SilentStatementType::VarInit:
                if(statement.variable->isReference)
                {
                    switch(statement.variable->size)
                    {
                        case 1:
                            code.AddNumber<char>((char)SilentBytecode::Alloc1);
                        break;
                        case 2:
                            code.AddNumber<char>((char)SilentBytecode::Alloc2);
                        break;
                        case 4:
                            code.AddNumber<char>((char)SilentBytecode::Alloc4);
                        break;
                        case 8:
                            code.AddNumber<char>((char)SilentBytecode::Alloc8);
                        break;
                        default:
                        {
                            code.AddNumber<char>((char)SilentBytecode::AllocX);
                            code.AddNumber<uint64>(statement.variable->size);
                        }
                    }
                }
                else code.AddPush(statement.variable->type,"0");
            break;

            case SilentStatementType::Delete:
            {
                code.AddNumber<char>((char)SilentBytecode::Free);
                code.AddNumber<uint64>(statement.variable->localPos);
            }

            case SilentStatementType::Expression:
                CompileExpression(*statement.expression);
            break;

            case SilentStatementType::Return:
                code.AddNumber<char>((char)SilentBytecode::Return);
                //code.AddNumber<uint64>(1ll);
                code.AddNumber<uint64>(statement.dataType.size);
            break;

            default: break;
        }
        
        #if DEBUG
        std::cout << "Done compiling statement\n";
        #endif
    }

    void SilentCodeGenerator::CompileLocalScope(SilentLocalScope &scope)
    {
        #if DEBUG
        std::cout << "Compiling local scope\n";
        #endif
    
        std::vector<uint64> ifEndings;

        for(SilentStatement* statement : scope.statements)
        {
            switch(statement->type)
            {
                case SilentStatementType::If:
                {
                    uint64 ifNotGoto;
                    CompileIfStatement(*statement, &ifNotGoto);
                    if(ifNotGoto > 0) ifEndings.push_back(ifNotGoto);
                    //Resolve if goto statements
                    if(statement->ifStatement->hasNext == false)
                    {
                        if(ifEndings.size() > 0)
                        {
                            uint64 codePtr = code.GetCodePointer();
                            for(uint64 i : ifEndings)
                            {
                                #if DEBUG
                                std::cout << "Resolving\n";
                                std::cout << "Code PTR: " 
                                << code.GetCodePointer() << "\n";
                                std::cout << "Code i: " << i << "\n";
                                #endif
                                memcpy(code.GetPtrToCode()->data()+i,
                                    &codePtr,8);
                            }
                        }
                    }
                }
                break;
                default: CompileStatement(*statement); break;
            }
        }
        

        #if DEBUG
        std::cout << "Done compiling local scope\n";
        #endif
    }

    void SilentCodeGenerator::CompileFunction(SilentFunction &func)
    {
        #if DEBUG
        std::cout << "Compiling function: " << func.name << "\n";
        #endif

        symTable[&func] = code.GetCodePointer();

        if(func.name == "main")
        {
            char* codeData = code.GetPtrToCode()->data();
            uint64 funcPtr = code.GetCodePointer();
            memcpy(codeData + 1, &funcPtr, 8);
        }

        //std::cout << "Scope name: " << GenScopeName(func.name) << "\n";
        currentDataType = func.returnType;
        CompileLocalScope(*func.scope);

        //Clean up references (skip parameters)
        // for(uint64 i = func.parameterCount; i<func.scope->variables.size();i++)
        // {
        //     if(func.scope->variables[i]->isReference)
        //     {
        //         //code.AddNumber<char>((char)SilentBytecode::Load8);
        //         code.AddNumber<char>((char)SilentBytecode::Free);
        //         code.AddNumber<uint64>(func.scope->variables[i]->localPos);
        //     }
        // }

        #if DEBUG
        std::cout << "Done compiling function: " << func.name << "\n";
        #endif
    }
    void SilentCodeGenerator::CompileNamespace(SilentNamespace &scope)
    {
        #if DEBUG
        std::cout << "Compiling namespace: " << scope.name << "\n";
        #endif

        namespaces.push_back(&scope);

        //Compile nested namespace
        for(SilentNamespace* nScope : scope.namespaces)
            CompileNamespace(*nScope);

        //Compile namespace functions
        for(SilentFunction* func : scope.functions)
            CompileFunction(*func);

        namespaces.pop_back();

        #if DEBUG
        std::cout << "Done compiling namespace: " << scope.name << "\n";
        #endif
    }
}