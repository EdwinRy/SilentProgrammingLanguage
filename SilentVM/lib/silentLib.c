#include <stdio.h>
#include "../src/SilentVM2.h"
void outputInt(SilentVM* vm)
{
    printf("output: %i\n", vm->memory->stack[vm->memory->stackPointer]);
}

void outputStr(SilentVM* vm)
{
    printf("sp: %i\n", vm->memory->stackPointer);
    printf("output: %s\n", vm->memory->stack + vm->memory->stackPointer);
}