#include "SilentVM.h"
#include "SilentStack.h"

#define DATATOINT(memory,location, x) (x) = *(int*)((memory) + (location));
#define DATATOFLOAT(memory,location, x) (x) = *(float*)((memory) + (location));

#define Bytecode_Halt 0
#define Bytecode_GoTo 1
#define Bytecode_Call 2

#define Bytecode_ClearMemory 3
#define Bytecode_ClearStorage 4

#define Bytecode_PushByte 5
#define Bytecode_PushInt 6
#define Bytecode_PushFloat 7

#define Bytecode_PopByte 8
#define Bytecode_PopInt 9
#define Bytecode_PopFloat 10

#define Bytecode_StoreByte 11
#define Bytecode_StoreInt 12
#define Bytecode_StoreFloat 13

#define Bytecode_LoadByte 14
#define Bytecode_LoadInt 15
#define Bytecode_LoadFloat 16

#define Bytecode_SetByte 17
#define Bytecode_SetInt 18
#define Bytecode_SetFloat 19

#define Bytecode_AddByte 20
#define Bytecode_AddInt 21
#define Bytecode_AddFloat 22

#define Bytecode_SubtractByte 23
#define Bytecode_SubtractInt 24
#define Bytecode_SubtractFloat 25

#define Bytecode_MultiplyByte 26
#define Bytecode_MultiplyInt 27
#define Bytecode_MultiplyFloat 28

#define Bytecode_DivideByte 29
#define Bytecode_DivideInt 30
#define Bytecode_DivideFloat 31

#define Bytecode_Byte2Int 32
#define Bytecode_Byte2Float 33
#define Bytecode_Int2Float 34
#define Bytecode_Float2Int 35

#define Bytecode_SmallerThan 36
#define Bytecode_BiggerThan 37
#define Bytecode_Equal 38

#define Bytecode_If 39
#define Bytecode_IfNot 40

#define Bytecode_And 41
#define Bytecode_Or 42
#define Bytecode_Not 43



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
        switch (vm->script[vm->programCounter]) 
        {
        case Bytecode_Halt:
            vm->running = 0;

        case Bytecode_GoTo:
            vm->programCounter = vm->script[vm->programCounter + 1];

        case Bytecode_Call:
            //vm->programCounter++;
            vm->programCounter = *(int*)((vm->script)+(++(vm->programCounter)));

  
        case Bytecode_ClearMemory:
            memset(vm->stack->memory, 0, vm->stack->stackPointer);
            vm->stack->stackPointer = 0;

        case Bytecode_ClearStorage:
            memset(vm->stack->storage, 0, vm->stack->storagePointer);
            vm->stack->storagePointer = 0;


        case Bytecode_PushByte:
            vm->stack->memory[vm->stack->stackPointer++] = vm->script[++(vm->programCounter)];

        case Bytecode_PushInt:
            //Copy 4 bytes of data over
            memcpy(vm->stack->memory + vm->stack->stackPointer, vm->script + ++(vm->programCounter), 4);
            vm->stack->stackPointer += 4;

        case Bytecode_PushFloat:
            memcpy(vm->stack->memory + vm->stack->stackPointer, vm->script + ++(vm->programCounter), 4);
            vm->stack->stackPointer += 4;


        case Bytecode_PopByte:
            vm->stack->stackPointer--;

        case Bytecode_PopInt:
            vm->stack->stackPointer -= 4;

        case Bytecode_PopFloat:
            vm->stack->stackPointer -= 4;


        case Bytecode_StoreByte:
            memcpy(vm->stack->storage + vm->stack->storagePointer++, vm->script + ++(vm->programCounter), 1);

        case Bytecode_StoreInt:
            memcpy(vm->stack->storage + vm->stack->storagePointer++, vm->script + ++(vm->programCounter), 4);

        case Bytecode_StoreFloat:
            memcpy(vm->stack->storage + vm->stack->storagePointer++, vm->script + ++(vm->programCounter), 4);


        case Bytecode_LoadByte:
            LoadByte(vm);

        case Bytecode_LoadInt:
            LoadIntFloat(vm);

        case Bytecode_LoadFloat:
            LoadIntFloat(vm);


        case Bytecode_SetByte:
            SetByte(vm);

        case Bytecode_SetInt:
            SetIntFloat(vm);

        case Bytecode_SetFloat:
            SetIntFloat(vm);


        case Bytecode_AddByte:
            AddByte(vm);

        case Bytecode_AddInt:
            AddIntFloat(vm);

        case Bytecode_AddFloat:
            AddIntFloat(vm);


        case Bytecode_SubtractByte:
            SubtractByte(vm);

        case Bytecode_SubtractInt:
            SubtractIntFloat(vm);

        case Bytecode_SubtractFloat:
            SubtractIntFloat(vm);


        case Bytecode_MultiplyByte:
            MultiplyByte(vm);

        case Bytecode_MultiplyInt:
            MultiplyIntFloat(vm);

        case Bytecode_MultiplyFloat:
            MultiplyIntFloat(vm);


        case Bytecode_DivideByte:
            DivideByte(vm);

        case Bytecode_DivideInt:
            DivideIntFloat(vm);

        case Bytecode_DivideFloat:
            DivideIntFloat(vm);


        case Bytecode_Byte2Int:
            Byte2Int(vm);

        case Bytecode_Byte2Float:
            Byte2Float(vm);

        case Bytecode_Int2Float:
            Int2Float(vm);

        case Bytecode_Float2Int:
            Float2Int(vm);


        case Bytecode_SmallerThan:
            SmallerThan(vm);

        case Bytecode_BiggerThan:
            BiggerThan(vm);

        case Bytecode_Equal:
            Equal(vm);


        case Bytecode_If:
            If(vm);

        case Bytecode_IfNot:
            IfNot(vm);


        case Bytecode_And:
            And(vm);

        case Bytecode_Or:
            Or(vm);

        case Bytecode_Not:
            Not(vm);

        }

        vm->programCounter += 1;
    }

    ClearMemory(vm);
    ClearStorage(vm);
    free(vm->stack);
    free(vm);
}