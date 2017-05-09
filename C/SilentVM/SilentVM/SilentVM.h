#pragma once
#ifndef SILENTVM
#define SILENTVM


typedef struct SilentStack {

    char* memory;
    char* storage;

}SilentStack;


typedef struct SilentVM {

    SilentStack stack;

}SilentVM;


void CreateSilentVM(SilentVM *vm, SilentStack *stack, int *ScriptLength);
void DeleteSilentVM(SilentVM *vm);

void CreateSilentStack(SilentStack *stack, int stackSize);
void DeleteSilentStack(SilentStack *stack);

void UpdateStackSize(SilentStack *stack, int newStackSize);


#endif // !SilentVM



