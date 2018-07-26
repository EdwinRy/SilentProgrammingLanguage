#include <stdio.h>
#include "../src/SilentVM2.h"
void outputInt(SilentVM* vm)
{
    printf("output: %i\n",vm->memory->stack[vm->memory->stackPointer]);
}