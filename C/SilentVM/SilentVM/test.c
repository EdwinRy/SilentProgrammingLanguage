#include "SilentVM.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    SilentStack* stack = CreateSilentStack(50, 50);
    SilentVM* vm = CreateSilentVM(stack);

    char* script = malloc(10);
    script[0] = 5;
    script[1] = 2;
    script[2] = 5;
    script[3] = 2;
    script[4] = 20;
    script[5] = 3;
    script[6] = 0;
    script[7];
    script[8];
    script[9];

    ExecuteScript(vm, script);

    DeleteSilentStack(stack);
    DeleteSilentVM(vm);

    while(1){}
    return 0;
}