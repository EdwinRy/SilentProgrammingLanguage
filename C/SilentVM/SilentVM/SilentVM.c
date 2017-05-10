#include "SilentVM.h"

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
}


//Bytecode Operations
void Halt(SilentVM  *vm) 
{
    vm->running = 0;
}
void GoTo(SilentVM *vm, int *line)
{
    vm->programCounter = line;
}

//Silent stack memory
void ClearMemory(SilentStack *stack)
{
    free(stack->memory);
}
void ClearStorage(SilentStack *stack)
{
    free(stack->storage);
}

void PushByte(SilentVM *vm)
{
    vm->programCounter += 1;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1;
}
void PushInt(SilentVM *vm)
{
    vm->programCounter += 1;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1; 

    vm->programCounter += 1;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1;

    vm->programCounter += 1;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1;

    vm->programCounter += 1;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1;
}
void PushFloat(SilentStack *stack)
{

}

void PopByte(SilentStack *stack)
{

}
void PopInt(SilentStack *stack)
{

}
void PopFloat(SilentStack *stack)
{

}

//Silent stack storage
void StoreByte(SilentStack *stack)
{

}
void StoreInt(SilentStack *stack)
{

}
void StoreFloat(SilentStack *stack)
{

}

void LoadByte(SilentStack *stack)
{

}
void LoadInt(SilentStack *stack)
{

}
void LoadFloat(SilentStack *stack)
{

}

void SetByte(SilentStack *stack)
{

}
void SetInt(SilentStack *stack)
{

}
void SetFloat(SilentStack *stack)
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
