#include "SilentVM.h"
#include <stdio.h>

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

SilentVM* CreateSilentVM(SilentStack *stack)
{
    SilentVM* vm = malloc(sizeof(SilentVM));
    vm->script = malloc(1);
    vm->stack = stack;
	vm->programCounter = 0;
    return vm;
}

void DeleteSilentVM(SilentVM * vm)
{
    free(vm->script);    
    free(vm);
}

SilentStack* CreateSilentStack(int MemorySize, int StorageSize)
{
    SilentStack* stack = malloc(sizeof(SilentStack));
    stack->memory = malloc(MemorySize);
    stack->storage = malloc(StorageSize);
    stack->stackPointer = 0;
    stack->storagePointer = 0;
    return stack;
}


void DeleteSilentStack(SilentStack *stack)
{
    free(stack->memory);
    free(stack->storage);
    free(stack);
}

void UpdateStackSize(SilentStack *stack, int NewStackSize)
{
    realloc(stack->memory, NewStackSize);
}

void UpdateStorageSize(SilentStack *stack, int NewStackSize)
{
    realloc(stack->storage, NewStackSize);
}

void AddFunction(SilentVM* vm, void (*FunctionPointer[])()) 
{
    FunctionPointer[1]();
}

void ExecuteScript(SilentVM *vm, char *script)
{
    free(vm->script);
    vm->script = script;
    vm->running = 1;

    while (vm->running) {

        switch (*(vm->script + vm->programCounter)) 
        {
        case Bytecode_Halt:
            printf("stop\n");
            vm->running = 0;
			break;

        case Bytecode_GoTo:
            vm->programCounter = vm->script[vm->programCounter + 1];
			break;
			
        case Bytecode_Call:
            //vm->programCounter++;
            vm->programCounter = *(int*)((vm->script)+(++(vm->programCounter)));
			break;
			
  
        case Bytecode_ClearMemory:
            printf("clear\n");
            memset(vm->stack->memory, 0, vm->stack->stackPointer);
            vm->stack->stackPointer = 0;
			break;
			
        case Bytecode_ClearStorage:
            memset(vm->stack->storage, 0, vm->stack->storagePointer);
            vm->stack->storagePointer = 0;
			break;
			

        case Bytecode_PushByte:
            vm->stack->memory[vm->stack->stackPointer++] = vm->script[++vm->programCounter];

            printf("push\n");
			break;
			
        case Bytecode_PushInt:
            //Copy 4 bytes of data over
            memcpy(vm->stack->memory + vm->stack->stackPointer, vm->script + ++(vm->programCounter), 4);
            vm->stack->stackPointer += 4;
			break;
			
        case Bytecode_PushFloat:
            memcpy(vm->stack->memory + vm->stack->stackPointer, vm->script + ++(vm->programCounter), 4);
            vm->stack->stackPointer += 4;
			break;
			

        case Bytecode_PopByte:
            vm->stack->stackPointer--;
			break;
			
        case Bytecode_PopInt:
            vm->stack->stackPointer -= 4;
			break;
			
        case Bytecode_PopFloat:
            vm->stack->stackPointer -= 4;
			break;
			

        case Bytecode_StoreByte:
            memcpy(vm->stack->storage + vm->stack->storagePointer++, vm->script + ++(vm->programCounter), 1);
			break;
			
        case Bytecode_StoreInt:
            memcpy(vm->stack->storage + vm->stack->storagePointer++, vm->script + ++(vm->programCounter), 4);
			break;
			
        case Bytecode_StoreFloat:
            memcpy(vm->stack->storage + vm->stack->storagePointer++, vm->script + ++(vm->programCounter), 4);
			break;
			

        case Bytecode_LoadByte:
            //LoadByte(vm);
			break;
			
        case Bytecode_LoadInt:
            //LoadIntFloat(vm);
			break;
			
        case Bytecode_LoadFloat:
            //LoadIntFloat(vm);
			break;
			

        case Bytecode_SetByte:
            //SetByte(vm);
			break;
			
        case Bytecode_SetInt:
            //SetIntFloat(vm);
			break;
			
        case Bytecode_SetFloat:
            //SetIntFloat(vm);
			break;
			

        case Bytecode_AddByte:
            printf("add\n");
            //AddByte(vm);
			break;
			
        case Bytecode_AddInt:
            //AddIntFloat(vm);
			break;
			
        case Bytecode_AddFloat:
            //AddIntFloat(vm);
			break;
			

        case Bytecode_SubtractByte:
            //SubtractByte(vm);
			break;
			
        case Bytecode_SubtractInt:
            //SubtractIntFloat(vm);
			break;
			
        case Bytecode_SubtractFloat:
            //SubtractIntFloat(vm);
			break;
			
	
        case Bytecode_MultiplyByte:
            //MultiplyByte(vm);
			break;
			
        case Bytecode_MultiplyInt:
            //MultiplyIntFloat(vm);
			break;
			
        case Bytecode_MultiplyFloat:
            //MultiplyIntFloat(vm);
			break;
			

        case Bytecode_DivideByte:
            //DivideByte(vm);
			break;
			
        case Bytecode_DivideInt:
            //DivideIntFloat(vm);
			break;
			
        case Bytecode_DivideFloat:
            //DivideIntFloat(vm);
			break;
			

        case Bytecode_Byte2Int:
            //Byte2Int(vm);
			break;
			
        case Bytecode_Byte2Float:
            //Byte2Float(vm);
			break;
			
        case Bytecode_Int2Float:
            //Int2Float(vm);
			break;
			
        case Bytecode_Float2Int:
            //Float2Int(vm);
			break;
			

        case Bytecode_SmallerThan:
            //SmallerThan(vm);
			break;
			
        case Bytecode_BiggerThan:
            //BiggerThan(vm);
			break;
			
        case Bytecode_Equal:
            //Equal(vm);
			break;
			

        case Bytecode_If:
            //If(vm);
			break;
			
        case Bytecode_IfNot:
            //IfNot(vm);
			break;
			

        case Bytecode_And:
            //And(vm);
			break;
			
        case Bytecode_Or:
            //Or(vm);
			break;
			
        case Bytecode_Not:
            //Not(vm);
			break;
			
        }

        vm->programCounter += 1;
    }
}
