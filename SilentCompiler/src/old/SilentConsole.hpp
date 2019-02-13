#pragma once
#include <iostream>
namespace Silent
{
    class SilentConsole
    {
        private:
        bool running;

        public:
        SilentConsole();
        void Start();
        void Exec(std::string statement);
        void Stop();
    };
}