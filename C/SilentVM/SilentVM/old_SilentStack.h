#pragma once
#include "SilentVM.h"

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