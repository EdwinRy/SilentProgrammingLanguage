#include <iostream>
class SilentCompiler
{
    public:
    
    private:
    bool success;
    bool outputAssembly;
    bool outputBytecode;
    std::string inFilePath;
    std::string outFilePath;
    std::string source;
    std::string bytecodeOutput;
    std::string assemblyOutput;

    public:
    SilentCompiler();
    ~SilentCompiler();
    void compile();
    void setInFile(std::string path);
    void setOutFile(std::string path);
    void setInSource(std::string source);
    void setOutputAssembly(bool flag);
    void setOutputBytecode(bool flag);
    std::string getBytecodeOutput();
    std::string getBytecodeOutput();
    bool getSuccessfull();
};