#pragma once
#include "SilentVM.h"


void GoTo(SilentVM *vm, int position);

void ClearMemory(SilentStack *stack);
void ClearStorage(SilentStack *stack);

void PushByte(SilentStack *vm, char* data);
void PushInt(SilentStack *vm, int* data);
void PushFloat(SilentStack *vm, float* data);
char* PopByte(SilentStack *stack);
int* PopInt(SilentStack *stack);
float* PopFloat(SilentStack *stack);

void StoreByte(SilentStack *vm);
void StoreIntFloat(SilentStack *vm);
void LoadByte(SilentStack *vm);
void LoadIntFloat(SilentStack *vm);
void SetByte(SilentStack *vm);
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

