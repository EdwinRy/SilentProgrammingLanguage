#include "SilentVM.h"
#include <stdlib.h>
#include <stdio.h>

void swag() 
{
    printf("first kek\n");
}

void sweg() 
{
    printf("second kek\n");
}

int main() 
{

    SilentStack* stack = CreateSilentStack(50, 50);
    SilentVM* vm = CreateSilentVM(stack);

    PrepareFunctions(vm, 2);

    void(**FunctionPointers)(SilentStack* stack) = malloc(sizeof(*FunctionPointers) * 2);
    FunctionPointers[0] = &swag;
    FunctionPointers[1] = &sweg;

    AddFunctions(vm, FunctionPointers);

    char* script = malloc(20);
    script[0] = 1;
    script[1] = 5;
    script[2] = 0;
    script[3] = 0;
    script[4] = 0;
    script[5] = 2;
    script[6] = 0;
    script[7] = 0;
    script[8] = 0;
    script[9] = 0;
    script[10] = 2;
    script[11] = 1;
    script[12] = 0;
    script[13] = 0;
    script[14] = 0;
    script[15] = 0;
    script[16] = 3;
    script[17] = 0;

    ExecuteScript(vm, script);

    DeleteSilentStack(stack);
    DeleteSilentVM(vm);

    printf("\ndone\n");
    getchar();
    //while(1){}
    return 0;
}