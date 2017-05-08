#pragma once
#include <vector>
#include <list>

namespace SilentVM {

    class Stack {

    public:
        int stackPointer = 0;
        std::list<char> *memory = new std::list<char>();
        std::vector<char> *storage = new std::vector<char>();

        Stack();

        //Memory Operations
        void PushInt(char* data);
        void PushFloat();
        void PushByte();

        void PopInt();
        void PopFloat();
        void PopByte();

        //Storage Operations
        void StoreInt();
        void StoreFloat();
        void StoreByte();

        void LoadInt();
        void LoadFloat();
        void LoadByte();

        //Math Operations
        void Add();
        void Subtract();
        void Multiply();
        void Divide();

        //Logical Operations
        void SmallerThan();
        void LargerThan();
        void Equal();

        //Bit Operations
        void And();
        void Or();
        void Not();

        void ShiftLeft();
        void ShiftRight();


    };

}