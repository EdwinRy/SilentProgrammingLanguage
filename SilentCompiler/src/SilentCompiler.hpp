#pragma once
#include <iostream>
namespace Silent
{
    class SilentCompiler
    {
        private:
        std::string source;
        char* path;

        std::string output;

        public:
        SilentCompiler();
        void Compile();
        void SetSource(char* source);
        void SetPath(char* path);
    };
}