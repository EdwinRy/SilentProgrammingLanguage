#pragma once
#include <iostream>
namespace Silent
{
    enum class SilentCompileMode
    {
        Asm,Src
    };
    class SilentCompiler
    {
        private:
        std::string source;
        char* path;

        std::string output;

        public:
        SilentCompiler();
        void Compile(SilentCompileMode mode);
        void SetSource(char* source);
        void SetPath(char* path);
    };
}