#include "SilentVM.h"
#include "SilentStack.h"

#define Bytecode_Halt 0
#define Bytecode_Goto 1
#define Bytecode_Call 2
#define Bytecode_ClearMemory 3
#define Bytecode_ClearMemory 4

#define Bytecode_PushByte 5
#define Bytecode_PushIntFloat 6

#define Bytecode_PopByte 7
#define Bytecode_PopIntFloat 8
#define Bytecode_PopBackByte 9
#define Bytecode_PopBackIntFloat 10

#define Bytecode_StoreByte 11
#define Bytecode_StoreIntFloat 12
#define Bytecode_LoadByte 13
#define Bytecode_LoadIntFloat 14
#define Bytecode_SetByte 15
#define Bytecode_SetIntFloat 16
#define Bytecode_DeleteByte 17
#define Bytecode_DeleteIntFloat 18

#define Bytecode_AddByte 19
#define Bytecode_AddIntFloat 20
#define Bytecode_SubtractByte 21
#define Bytecode_SubtractIntFloat 22
#define Bytecode_MultiplyByte 23
#define Bytecode_MultiplyIntFloat 24
#define Bytecode_DivideByte 25
#define Bytecode_DivideIntFloat 26

#define Bytecode_Byte2Int 27
#define Bytecode_Byte2Float 28
#define Bytecode_Int2Float 29
#define Bytecode_Float2Int 30

#define Bytecode_SmallerThan 31
#define Bytecode_BiggerThan 32
#define Bytecode_Equal 33

#define Bytecode_If 34
#define Bytecode_IfNot 35

#define Bytecode_And 36
#define Bytecode_Or 37
#define Bytecode_Not 38



//Bytecode Operations
void Halt(SilentVM  *vm) 
{
    vm->running = 0;
}
void GoTo(SilentVM *vm)
{
    vm->programCounter = vm->script[vm->programCounter+1];
}
void Call(SilentVM *vm)
{
    vm->programCounter++;
    int *pos = malloc(4);
    memcpy(pos, vm->script + vm->programCounter, 4);
    vm->programCounter = *pos;
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
    vm->programCounter ++;
    vm->stack->memory[vm->stack->stackPointer] = vm->script[vm->programCounter];
    vm->stack->stackPointer += 1;
}

//Push either float or integer
void PushIntFloat(SilentVM *vm)
{
    vm->programCounter ++;

    //Copy 4 bytes of data over
    memcpy(vm->stack->memory + vm->stack->stackPointer, vm->script + vm->programCounter, 4);

    vm->stack->stackPointer += 4;
}


void PopByte(SilentStack *stack)
{
    //memset(stack->memory - 1, 0, 1);
    stack->stackPointer --;
}
void PopIntFloat(SilentStack *stack)
{
    //memset(stack->memory - 4, 0, 4);
    stack->stackPointer -= 4;
}
void PopBackByte(SilentStack * stack)
{
}
void PopBackIntFloat(SilentStack * stack)
{
}


//Silent stack storage
void StoreByte(SilentVM *vm)
{
    vm->programCounter++;
    memcpy(vm->stack->storage + vm->stack->storagePointer, vm->script + vm->programCounter, 1);
    vm->stack->storagePointer++;
}
void StoreIntFloat(SilentVM *vm)
{
    vm->programCounter++;
    memcpy(vm->stack->storage + vm->stack->storagePointer, vm->script + vm->programCounter, 4);
    vm->stack->storagePointer += 4;
}

void LoadByte(SilentVM *vm)
{
    vm->programCounter++;
    int *pos = malloc(4);
    pos = memcpy(pos, vm->script + vm->programCounter, 1);

    memcpy(
        vm->stack->memory + vm->stack->stackPointer,
        vm->stack->storage + *pos,
        1
    );
    vm->stack->stackPointer++;
    free(pos);
}
void LoadIntFloat(SilentVM *vm)
{
    vm->programCounter++;
    int *pos = malloc(4);
    memcpy(pos, vm->script + vm->programCounter, 4);

    memcpy(
        vm->stack->memory + vm->stack->stackPointer,
        vm->stack->storage + *pos,
        4
    );
    vm->stack->stackPointer += 4;
    vm->programCounter += 4;
    free(pos);
}

void SetByte(SilentVM *vm)
{
    vm->programCounter++;
    int *pos = malloc(4);
    memcpy(pos, vm->script + vm->programCounter, 4);
    memcpy(vm->stack->storage + *pos, vm->stack->memory + vm->stack->stackPointer);


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
void AddIntFloat(SilentStack *stack) 
{
}

void SubtractByte(SilentStack * stack)
{
}
void SubtractIntFloat(SilentStack * stack)
{
}

void MultiplyByte(SilentStack * stack)
{
}
void MultiplyIntFloat(SilentStack * stack)
{
}

void DivideByte(SilentStack * stack)
{
}
void DivideIntFloat(SilentStack * stack)
{
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

void If(SilentStack *stack)
{
}
void IfNot(SilentStack *stack)
{

}

//Silent Bit operations
void And(SilentStack *stack){}
void Or(SilentStack *stack){}
void Not(SilentStack *stack){}



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