#include "SilentVM.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    SilentStack* stack = CreateSilentStack(50, 50);
    SilentVM* vm = CreateSilentVM(stack);

    DeleteSilentStack(stack);
    DeleteSilentVM(vm);
    return 0;
}