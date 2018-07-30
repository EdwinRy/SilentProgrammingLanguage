#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "SilentAssembler.hpp"
using namespace Silent;
typedef unsigned long long uint64;
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

void SilentAssembler::Assemble(char* path)
{
    std::ifstream inFile(path);
    std::string line;
    while(std::getline(inFile, line))
    {
        // //std::cout << tokenizeLine(line) << "\n";
        // std::vector<std::string> instructions = tokenizeLine(line);
        // for(uint64 i = 0; i < instructions.size(); i++)
        // {
        //     std::cout << "l " << instructions[i] << "\n";
        // }

        // std::cout << "endl" << "\n";

    }
} 

std::string SilentAssembler::GetOutput()
{
    return this->output;
}