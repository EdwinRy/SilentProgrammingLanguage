#pragma once
#include <iostream>
namespace Silent
{
    class SilentAssembler
    {
        private:
        std::vector<char*> output;
        char* path;

        public:
        SilentAssembler();
        void Assemble(char* path);
        std::string GetOutput();
    };
}