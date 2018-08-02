#pragma once
#include <iostream>
namespace Silent
{
    class SilentAssembler
    {
        private:
        char* output;
        char* path;

        public:
        SilentAssembler();
        void Assemble(char* path);
        void Link(std::vector<char> in);
        char* GetOutput();
    };
}