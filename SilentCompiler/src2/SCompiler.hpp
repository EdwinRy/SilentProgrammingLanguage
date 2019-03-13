#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    class SilentCompiler
    {
        private:
        std::string source;

        std::vector<char*> inFiles;
        char* outPath;

        std::string output;

        public:
        SilentCompiler();
        void Compile(std::vector<char*> inFiles, char* outPath);
        void CompileSource(std::string source, char* outFile);
        std::string GetOutput(){return output;}
    };
}