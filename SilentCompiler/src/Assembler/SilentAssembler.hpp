#pragma once
#include <iostream>
namespace Silent
{
    class SilentAssembler
    {
        private:
        std::string output;
        char* path;

        public:
        SilentAssembler();
        void Assemble(char* path);
        std::string GetOutput();
    };
}