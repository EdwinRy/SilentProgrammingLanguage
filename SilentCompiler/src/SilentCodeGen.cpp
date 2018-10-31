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

    SilentVMType SilentCode::ToVMType(SilentPrimitives p)
    {
        switch(p)
        {
            case SilentPrimitives::int8: return SilentVMType::INT8; break;
            case SilentPrimitives::uint8: return SilentVMType::UINT8; break;
            case SilentPrimitives::int16: return SilentVMType::INT16; break;
            case SilentPrimitives::uint16: return SilentVMType::UINT16; break;
            case SilentPrimitives::int32: return SilentVMType::INT32; break;
            case SilentPrimitives::uint32: return SilentVMType::UINT32; break;
            case SilentPrimitives::int64: return SilentVMType::INT64; break;
            case SilentPrimitives::uint64: return SilentVMType::UINT64; break;
            case SilentPrimitives::float32: return SilentVMType::FLOAT32; break;
            case SilentPrimitives::float64: return SilentVMType::FLOAT64; break;
        }
    }

    SilentBytecode SilentCode::ToBytecodePush(SilentPrimitives p)
    {
        switch(p)
        {
            case SilentPrimitives::int8:
            case SilentPrimitives::uint8: return SilentBytecode::Push1; break;
            case SilentPrimitives::int16:
            case SilentPrimitives::uint16: return SilentBytecode::Push2; break;
            case SilentPrimitives::int32:
            case SilentPrimitives::uint32: 
            case SilentPrimitives::float32: return SilentBytecode::Push4; break;
            case SilentPrimitives::int64:
            case SilentPrimitives::uint64: 
            case SilentPrimitives::float64: return SilentBytecode::Push8; break;
        }
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

    SilentBytecode SilentCode::ToBytecodeLoad(SilentPrimitives p)
    {
        switch(p)
        {
            case SilentPrimitives::int8:
            case SilentPrimitives::uint8: return SilentBytecode::Load1; break;
            case SilentPrimitives::int16:
            case SilentPrimitives::uint16: return SilentBytecode::Load2; break;
            case SilentPrimitives::int32:
            case SilentPrimitives::uint32: 
            case SilentPrimitives::float32: return SilentBytecode::Load4; break;
            case SilentPrimitives::int64:
            case SilentPrimitives::uint64: 
            case SilentPrimitives::float64: return SilentBytecode::Load8; break;
        }
    }

    void SilentCode::AddPush(SilentDataType dt, std::string val)
    {
        if(dt.isPrimitive)
        {
            AddNumber<char>((char)ToBytecodePush(dt.primitive));
            AddNumber<char>((char)ToVMType(dt.primitive));
            AddData(dt, val);
        }
    }

    void SilentCode::AddLoad(SilentDataType dt, uint64 localPos)
    {
        if(dt.isPrimitive)
        {
            AddNumber<char>((char)ToBytecodeLoad(dt.primitive));
            AddNumber<char>((char)ToVMType(dt.primitive));
            AddNumber<uint64>(localPos);
        }
    }

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

    void SilentCodeGenerator::Compile(SilentParser *parser)
    {
        CompileNamespace(*parser->GetGlobalNamespace());
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
                currentType = 
                    expression.left->variable->type;
                printf("Assignment of type %i\n",(int)currentType.primitive);
            break;

            case SilentOperandType::Add:

            break;

            case SilentOperandType::Subtract:
            break;

            case SilentOperandType::Multiply:
            break;

            case SilentOperandType::Divide:
            break;

            case SilentOperandType::Number:
                code.AddPush(currentType, expression.token->value);
            break;

            default: break;
        }
        #if DEBUG
        std::cout << "Done compiling expression\n";
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
                if(statement.variable->type.isPrimitive)
                {
                    switch(statement.variable->type.primitive)
                    {
                        case SilentPrimitives::int8:
                        code.AddNumber<char>((char)SilentBytecode::Push1);  
                        code.AddNumber<char>((char)SilentVMType::INT8);
                        code.AddNumber<char>(0);
                        break;

                        case SilentPrimitives::uint8:
                        code.AddNumber<char>((char)SilentBytecode::Push1);  
                        code.AddNumber<char>((char)SilentVMType::UINT8);
                        code.AddNumber<char>(0);
                        break;

                        case SilentPrimitives::int16:
                        code.AddNumber<char>((char)SilentBytecode::Push2);  
                        code.AddNumber<char>((char)SilentVMType::INT16);
                        code.AddNumber<short>(0);
                        break;

                        case SilentPrimitives::uint16:
                        code.AddNumber<char>((char)SilentBytecode::Push2);  
                        code.AddNumber<char>((char)SilentVMType::UINT16);
                        code.AddNumber<short>(0);
                        break;

                        case SilentPrimitives::int32:
                        code.AddNumber<char>((char)SilentBytecode::Push4);  
                        code.AddNumber<char>((char)SilentVMType::INT32);
                        code.AddNumber<int>(0);
                        break;

                        case SilentPrimitives::uint32:
                        code.AddNumber<char>((char)SilentBytecode::Push4);  
                        code.AddNumber<char>((char)SilentVMType::UINT32);
                        code.AddNumber<int>(0);
                        break;

                        case SilentPrimitives::int64:
                        code.AddNumber<char>((char)SilentBytecode::Push8);  
                        code.AddNumber<char>((char)SilentVMType::INT64);
                        code.AddNumber<long long>(0);
                        break;

                        case SilentPrimitives::uint64:
                        code.AddNumber<char>((char)SilentBytecode::Push8);  
                        code.AddNumber<char>((char)SilentVMType::UINT64);
                        code.AddNumber<long long>(0);
                        break;

                        case SilentPrimitives::float32:
                        code.AddNumber<char>((char)SilentBytecode::Push4);  
                        code.AddNumber<char>((char)SilentVMType::INT64);
                        code.AddNumber<float>(0);
                        break;

                        case SilentPrimitives::float64:
                        code.AddNumber<char>((char)SilentBytecode::Push4);  
                        code.AddNumber<char>((char)SilentVMType::INT64);
                        code.AddNumber<double>(0);
                        break;

                        default: break;
                    }
                }
                else
                {
            
                }
            break;

            case SilentStatementType::Expression:
                CompileExpression(*statement.expression);
            break;

            case SilentStatementType::If:
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
    
        for(SilentStatement* statement : scope.statements)
        {
            CompileStatement(*statement);
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

        std::cout << "Scope name: " << GenScopeName(func.name) << "\n";
        CompileLocalScope(*func.scope);

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