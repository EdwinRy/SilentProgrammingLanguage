#pragma once
#include "SilentVM.h"

/*
void Halt(SilentVM  *vm);
void GoTo(SilentVM *vm);
void Call(SilentVM *vm);

void ClearMemory(SilentStack *stack);
void ClearStorage(SilentStack *stack);

void PushByte(SilentVM *vm);
void PushIntFloat(SilentVM *vm);
void PopByte(SilentStack *stack);
void PopIntFloat(SilentStack *stack);
void PopBackByte(SilentStack *stack);
void PopBackIntFloat(SilentStack *stack);

void StoreByte(SilentVM *vm);
void StoreIntFloat(SilentVM *vm);
void LoadByte(SilentVM *vm);
void LoadIntFloat(SilentVM *vm);
void SetByte(SilentVM *vm);
void SetIntFloat(SilentStack *stack);
void DeleteByte(SilentStack *stack);
void DeleteIntFloat(SilentStack *stack);

void AddByte(SilentStack *stack);
void AddIntFloat(SilentStack *stack);

void SubtractByte(SilentStack *stack);
void SubtractIntFloat(SilentStack *stack);

void MultiplyByte(SilentStack *stack);
void MultiplyIntFloat(SilentStack *stack);

void DivideByte(SilentStack *stack);
void DivideIntFloat(SilentStack *stack);

void Byte2Int(SilentStack *stack);
void Byte2Float(SilentStack *stack);
void Int2Float(SilentStack *stack);
void Float2Int(SilentStack *stack);

void SmallerThan(SilentStack *stack);
void BiggerThan(SilentStack *stack);
void Equal(SilentStack *stack);

void If(SilentStack *stack);
void IfNot(SilentStack *stack);

void And(SilentStack *stack);
void Or(SilentStack *stack);
void Not(SilentStack *stack);
*/

/*
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

*/