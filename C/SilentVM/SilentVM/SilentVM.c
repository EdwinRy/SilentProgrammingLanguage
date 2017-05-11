#include "SilentVM.h"

//Bytecode Operations
void Halt(SilentVM  *vm) 
{
    vm->running = 0;
}
void GoTo(SilentVM *vm)
{
    vm->programCounter = vm->script[vm->programCounter+1];
}

//Silent stack memory
void ClearMemory(SilentStack *stack)
{
    free(stack->memory);
    stack->stackPointer = 0;
}
void ClearStorage(SilentStack *stack)
{
    free(stack->storage);
    stack->storagePointer = 0;
}

void PushByte(SilentVM *vm)
{
    vm->programCounter += 1;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1;
}

//Push either float or integer
void PushIntFloat(SilentVM *vm)
{
    vm->programCounter += 1;

    //Copy 4 bytes of data over
    memcpy(vm->stack->memory + vm->stack->stackPointer, vm->script + vm->programCounter, 4);

    vm->stack->stackPointer += 5;
}


void PopByte(SilentStack *stack)
{
    memset(stack->memory - 1, 0, 1);
    stack->stackPointer -= 1;
}
void PopIntFloat(SilentStack *stack)
{
    memset(stack->memory - 4, 0, 4);
    stack->stackPointer -= 1;
}


//Silent stack storage
void StoreByte(SilentStack *stack)
{

}
void StoreIntFloat(SilentStack *stack)
{

}

void LoadByte(SilentStack *stack)
{

}
void LoadIntFloat(SilentStack *stack)
{

}

void SetByte(SilentStack *stack)
{

}
void SetIntFloat(SilentStack *stack)
{

}

void DeleteByte(SilentStack *stack)
{

}
void DeleteIntFloat(SilentStack *stack)
{

}

//Silent Maths operations
void AddByte(SilentStack *stack){

}
void AddInt(SilentStack *stack) {

}
void AddFloat(SilentStack *stack){

}

//Silent Variable Conversion
void Byte2Int(SilentStack *stack)
{

}
void Byte2Float(SilentStack *stack)
{

}
void Int2Float(SilentStack *stack)
{

}
void Float2Int(SilentStack *stack)
{

}

//Silent Logical operations
void SmallerThan(SilentStack *stack)
{

}
void BiggerThan(SilentStack *stack)
{

}
void Equal(SilentStack *stack)
{

}

//Silent Bit operations
void And(SilentStack *stack){}
void Or(SilentStack *stack){}
void Not(SilentStack *stack){}



void CreateSilentVM(SilentVM * vm, SilentStack *stack)
{
    vm->stack = stack;
}

void DeleteSilentVM(SilentVM * vm)
{
    free(vm);
}

void CreateSilentStack(SilentStack * stack, int MemorySize, int StorageSize)
{
    stack->memory = malloc(MemorySize);
    stack->storage = malloc(StorageSize);
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

void UpdateStorageSize(SilentStack *stack, int NewStackSize)
{
    realloc(stack->storage, NewStackSize);
}

void ExecuteScript(SilentVM *vm, char *script)
{
    vm->script = script;
    vm->running = 1;

    while (vm->running) {
        switch (vm->script[vm->programCounter]) {
        case 0:
            Halt(vm);
        }
        vm->programCounter += 1;
    }

    ClearMemory(vm);
    ClearStorage(vm);
    free(vm->stack);
    free(vm);
}