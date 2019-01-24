#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    class SilentCompiler
    {
        private:
        std::string source;
        // char* path;
        // char* libPath;

        char* inFile;
        char* outPath;

        std::string output;
        // std::string libOutput;

        public:
        SilentCompiler();
        void Compile(char* inFile, char* outPath);
        void CompileSource(std::string source, char* outFile);
        std::string GetOutput(){return output;}
    };
}