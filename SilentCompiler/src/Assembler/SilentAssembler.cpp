#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "SilentAssembler.hpp"
using namespace Silent;
typedef unsigned long long uint64;

enum class SilentDataType
{
	INT8 = 0,
	UINT8,
    INT16,
	UINT16,
    INT32,
	UINT32,
	INT64,
    UINT64,
    FLOAT32,
    FLOAT64,
    POINTER,
    POINTER_LOCATION,
	UNDEFINED,
	UNDEFINED_END
};

enum class SilentBytecode
{
	Halt = 0,
	Goto,
	Sweep,
	Call,
	Return,
	LoadDll,
	LoadDllFunc,
	FreeDll,
	CallDllFunc,
	PushData,
	Pop,
	Store,
	Load,
	StoreGlobal,
	LoadGlobal,
	Alloc,
	LoadPtr,
	StorePtr,
	FreePtr,
	Free,
	GetPtr,
	Add,
	Sub,	
	Mul,	
	Div,
	Convert,
	SmallerThan,
	LargerThan,
	Equal,
	If,
	IfNot,
	And,
	Or,
	Not
};

SilentAssembler::SilentAssembler()
{
    this->output = "";
}

std::vector<std::string> tokenizeLine(std::string line)
{
    std::vector<std::string> instructions;
    std::string buffer = "";

    for(uint64 i = 0; i < line.size(); i++)
    {
        //std::cout << "k " << line[i] << "\n";
        if(line[i] == '\"')
        {
            i++;
            while(line[i] != '\"')
            {
                buffer += line[i]; 
                //std::cout << "s " << line[i] << "\n";
                i += 1;
            }
            i+=1;
            instructions.push_back(buffer);
            buffer.clear();
            continue;
        }
        else if(line[i] == ' ')
        {
            instructions.push_back(buffer);
            buffer.clear();
            continue;
        }
        else
        {
            //std::cout << "c " << line[i] << "\n";
            buffer += line[i];
        }
    }
    if(buffer.size() != 0)
    {
        instructions.push_back(buffer);
        buffer.clear();
    }
    return instructions;
}

struct SilentLabel
{
    std::string label;
    uint64 index;
};

void SilentAssembler::Assemble(char* path)
{
    std::ifstream inFile(path);
    std::string line;
    uint64 currentLine = 0;
    std::vector<char*> program;
    std::vector<SilentLabel> labels;
    std::vector<SilentLabel> aLabels;
    while(std::getline(inFile, line))
    {
        currentLine++;
        std::vector<std::string> instructions = tokenizeLine(line);

        if(instructions.size() < 1)
        {
            continue;
        }

        if(instructions[0].back() == ':')
        {
            SilentLabel label;
            instructions[0].pop_back();
            label.label = instructions[0];
            label.index = program.size();
        }

        else if(instructions[0] == "halt")
        {
            program.push_back((char)SilentBytecode::Halt);
        }

        else if(instructions[0] == "goto")
        {
            char* temp = (char*)malloc(1);
            temp[0] = (char)SilentBytecode::Goto;
            SilentLabel label;
            label.label = instructions[1];
            label.index = program.size();
            aLabels.push_back(label);
            //program.insert(program.end(),&currentLine,std::end())
            program.push_back(temp);
        }
        else if(instructions[0] == "sweep")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "call")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "return")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "loaddll")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "loaddllfunc")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "freedll")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "calldllfunc")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "push")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "pop")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "store")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "load")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "storeglobal")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "loadglobal")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "alloc")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "loadptr")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "storeptr")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "getptr")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "freeptr")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "free")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "add")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "sub")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "mul")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "div")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "convert")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "smallerthan")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "largerthan")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "equal")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "if")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "ifn")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "and")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "or")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else if(instructions[0] == "not")
        {
            program.push_back((char)SilentBytecode::Halt);
        }
        else
        {
            std::cout << "Invalid instruction \"" 
            << instructions[0] << "\" on line " << currentLine << "\n";
        }

    }
} 

std::string SilentAssembler::GetOutput()
{
    return this->output;
}