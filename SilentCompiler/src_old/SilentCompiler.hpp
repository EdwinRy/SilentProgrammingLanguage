#pragma once
#include <iostream>
#include <vector>
class SilentCompiler
{
    public:
    std::string bytecodeOutput;
    std::vector<std::string> assemblyOutput;
    
    private:
    bool success;
    bool outputAssembly;
    bool outputBytecode;
    std::string inFilePath;
    std::string outFilePath;
    std::string source;
    std::string errorMessage;

    public:
    SilentCompiler();
    //~SilentCompiler();
    void compile();
    void setInFile(std::string path);
    void setOutFile(std::string path);
    void setInSource(std::string source);
    void setOutputAssembly(bool flag);
    void setOutputBytecode(bool flag);
    bool getSuccessfull();
};