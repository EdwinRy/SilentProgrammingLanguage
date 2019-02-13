#include "SilentConsole.hpp"
#include <iostream>

namespace Silent
{
    SilentConsole::SilentConsole()
    {
        this->running = true;
    }

    void SilentConsole::Start()
    {
        while(this->running)
        {
            std::string statement;
            std::cout << ">";
            std::cin >> statement;
            this->Exec(statement);
        }
    }

    void SilentConsole::Exec(std::string statement)
    {
        if(statement == "exit")
        {
            this->running = false;
        }
    }
}