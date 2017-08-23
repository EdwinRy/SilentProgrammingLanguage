#include "SilentVM.h"

#define BYTECODE_HALT 0
#define BYTECODE_GOTO 1
#define BYTECODE_CALL 2

#define BYTECODE_CLEAR_MEMORY 3
#define BYTECODE_CLEAR_STACK 4

#define BYTECODE_PUSH_BYTE 5
#define BYTECODE_PUSH_INT 6
#define BYTECODE_PUSH_LONG 7
#define BYTECODE_PUSH_FLOAT 8
#define BYTECODE_PUSH_DOUBLE 9

#define BYTECODE_POP_BYTE 10
#define BYTECODE_POP4 11
#define BYTECODE_POP8 12

#define BYTECODE_STORE_BYTE 13
#define BYTECODE_STORE_INT 14
#define BYTECODE_STORE_LONG 15
#define BYTECODE_STORE_FLOAT 16
#define BYTECODE_STORE_DOUBLE 17

#define BYTECODE_LOAD_BYTE 18
#define BYTECODE_LOAD_INT 19
#define BYTECODE_LOAD_LONG 20
#define BYTECODE_LOAD_FLOAT 21
#define BYTECODE_LOAD_DOUBLE 22

#define BYTECODE_ADD_BYTE 23
#define BYTECODE_ADD_INT 24
#define BYTECODE_ADD_LONG 25
#define BYTECODE_ADD_FLOAT 26
#define BYTECODE_ADD_DOUBLE 27

#define BYTECODE_SUB_BYTE 28
#define BYTECODE_SUB_INT 29
#define BYTECODE_SUB_LONG 30
#define BYTECODE_SUB_FLOAT 31
#define BYTECODE_SUB_DOUBLE 32

#define BYTECODE_MUL_BYTE 33
#define BYTECODE_MUL_INT 34
#define BYTECODE_MUL_LONG 35
#define BYTECODE_MUL_FLOAT 36
#define BYTECODE_MUL_DOUBLE 37

#define BYTECODE_DIV_BYTE 38
#define BYTECODE_DIV_INT 39
#define BYTECODE_DIV_LONG 40
#define BYTECODE_DIV_FLOAT 41
#define BYTECODE_DIV_DOUBLE 42

#define BYTECODE_BYTE_TO_INT 43
#define BYTECODE_BYTE_TO_LONG 44
#define BYTECODE_BYTE_TO_FLOAT 45
#define BYTECODE_BYTE_TO_DOUBLE 46

#define BYTECODE_INT_TO_BYTE 47
#define BYTECODE_INT_TO_FLOAT 48
#define BYTECODE_INT_TO_LONG 49
#define BYTECODE_INT_TO_DOUBLE 50

#define BYTECODE_FLOAT_TO_INT 51
#define BYTECODE_FLOAT_TO_DOUBLE 52

#define BYTECODE_SMALLER_THAN 53
#define BYTECODE_BIGGER_THAN 54
#define BYTECODE_EQUAL 55

#define BYTECODE_IF 56
#define BYTECODE_IF_NOT 57

int main() 
{

	SilentVM* vm;
	SilentThread* thread;
	SilentMemory* memory;

	char* bytecode = malloc(100);
	int x = 0;
	/*
	bytecode[x++] = BYTECODE_PUSH_INT;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 1;
	bytecode[x++] = BYTECODE_STORE_INT;
	bytecode[x++] = BYTECODE_PUSH_INT;
	bytecode[x++] = 0;
	bytecode[x++] = 1;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = BYTECODE_STORE_INT;
	bytecode[x++] = BYTECODE_CLEAR_MEMORY;
	bytecode[x++] = BYTECODE_PUSH_INT;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 1;
	bytecode[x++] = 0;
	bytecode[x++] = BYTECODE_STORE_INT;
	bytecode[x++] = BYTECODE_HALT;
	*/
	bytecode[x++] = BYTECODE_PUSH_LONG;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 1;
	bytecode[x++] = BYTECODE_STORE_LONG;
	bytecode[x++] = BYTECODE_PUSH_LONG;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 1;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = 0;
	bytecode[x++] = BYTECODE_STORE_LONG;
	bytecode[x++] = BYTECODE_HALT;


	memory = createSilentMemory(500, 500);
	thread = createSilentThread(bytecode, memory);
	vm = createSilentVM(thread, 1);
	loadProgramToSilentVM(vm, bytecode);
	startSilentVM(vm);

	printf("%i", *((int*)(memory->storage[0])));
	getchar();
	return 0;
}