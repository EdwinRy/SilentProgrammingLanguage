#include <iostream>
#include "Silent.hpp"



std::string file = R"(
// comment here
/* this is a 
multiline 
comment */
qctual content here
)";


using namespace Silent;
int main()
{
    SilentProgram program = CreateProgram((char*)file.c_str());
    
    std::cout << file;
}