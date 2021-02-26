#include "Silent.hpp"
#include "compiler.hpp"

namespace Silent
{
    SilentProgram CreateProgram(char* source)
    {
        SilentProgram program;

        Compiler comp;
        program.bytes = comp.GetBytes(source);

        return program;
    }
}