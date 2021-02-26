#pragma once
namespace Silent
{
    class SilentProgram
    {
        public:
        char* bytes;
    };

    SilentProgram CreateProgram(char* source);
}