#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    class SilentCompiler
    {
        private:
        std::string source;

        char* inFile;
        char* outPath;

        std::string output;

        public:
        SilentCompiler();
        void Compile(char* inFile, char* outPath);
        void CompileSource(std::string source, char* outFile);
        std::string GetOutput(){return output;}
    };
}