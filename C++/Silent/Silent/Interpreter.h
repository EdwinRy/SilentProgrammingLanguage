#pragma once

class Interpreter
{
private:

    int programCounter;
    int stackPointer;

public:
    void interpret(char *filepath);

};
