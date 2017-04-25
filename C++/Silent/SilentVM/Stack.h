#pragma once
#include <vector>

namespace SilentVM {

    class Stack {

    public:
        int stackPointer;
        std::vector<char> *memory;
        std::vector<char> *storage;

        Stack();

        void PushInt();
        void PushFloat();
        void PushChar();

    };

}