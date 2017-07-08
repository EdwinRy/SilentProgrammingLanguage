#include "SilentVM.h"

#define DATATOINTFLOAT(memory,location, x) (x) = *(int*)((memory) + (location));

SilentVM* CreateSilentVM(SilentStack *stack)
{
    SilentVM* vm = malloc(sizeof(SilentVM));
    vm->stack = stack;
    return vm;
}

void DeleteSilentVM(SilentVM * vm)
{
    free(vm->script);

    free(vm);
}

SilentStack* CreateSilentStack(int MemorySize, int StorageSize)
{
    SilentStack* stack = malloc(sizeof(stack));
    stack->memory = malloc(MemorySize);
    stack->storage = malloc(StorageSize);
    stack->stackPointer = 0;
    stack->storagePointer = 0;
}


void DeleteSilentStack(SilentStack *stack)
{
    free(stack->memory);
    free(stack->storage);
}

void UpdateStackSize(SilentStack *stack, int NewStackSize)
{
    realloc(stack->memory, NewStackSize);
}

void UpdateStorageSize(SilentStack *stack, char *NewStackSize)
{
    realloc(stack->storage, NewStackSize);
}

void LoadScript(SilentVM *vm, int* script) 
{

}

void ExecuteScript(SilentVM *vm) 
{

    int* ByteCode = malloc(20);

    ByteCode[0] = &&print2;

    goto print2;


print:
    printf("hello");

print2:
    printf("hi");

}