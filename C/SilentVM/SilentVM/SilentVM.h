#pragma once

#include <stdlib.h>

typedef struct SilentStack {

    char* memory;
    char* storage;
    char* storagePointers;
    unsigned int stackPointer;
    unsigned int storagePointer;

}SilentStack;


typedef struct SilentVM {

    SilentStack* stack;
    char* script;
    unsigned int programCounter;
    char running;

    void (**FunctionPointers)(SilentStack* stack);
    unsigned int FunctionCounter;

}SilentVM;


//SilentVM functions
SilentVM* CreateSilentVM(SilentStack *stack);
void DeleteSilentVM(SilentVM *vm);

SilentStack* CreateSilentStack(int stackSize, int StorageSize);
void DeleteSilentStack(SilentStack *stack);

void UpdateStackSize(SilentStack *stack, unsigned int newStackSize);
void UpdateStorageSize(SilentStack *stack, unsigned int newStackSize);

void PrepareFunctions(SilentVM* vm, unsigned int NumberOfFunctions);
void AddFunctions(SilentVM* vm, void (**FunctionPointer)(SilentStack* stack));
void ExecuteScript(SilentVM *vm, char* script);
