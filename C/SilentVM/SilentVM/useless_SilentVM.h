#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct SilentStack {

    char* memory;
    char* storage;
    int stackPointer;
    int storagePointer;

}SilentStack;


typedef struct SilentVM {

    SilentStack* stack;
    char* script;
    int programCounter;
    char running;

}SilentVM;


//SilentVM functions
SilentVM* CreateSilentVM(SilentStack *stack);
void DeleteSilentVM(SilentVM *vm);

SilentStack* CreateSilentStack(int MemorySize, int StorageSize);
void DeleteSilentStack(SilentStack *stack);

void UpdateStackSize(SilentStack *stack, int newStackSize);
void UpdateStorageSize(SilentStack *stack, int newStackSize);

void ExecuteScript(SilentVM *vm);