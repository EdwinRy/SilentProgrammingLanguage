#include "SilentVM.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    SilentStack* stack = CreateSilentStack(50, 50);
    SilentVM* vm = CreateSilentVM(stack);

    char* script = malloc(10);
    script[0] = 1;
    script[1] = 0;
    script[2] = 0;
    script[3] = 0;
    script[4] = 0;
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