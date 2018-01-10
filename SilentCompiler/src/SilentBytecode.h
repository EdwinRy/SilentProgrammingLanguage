#include "SilentParser.h"
//Bytecode used by the virtual machine
typedef enum SilentBytecode
{
	Halt,
	Goto, //X - Byte
	CallSys,//

	Call,
	Return,
	
	//ClearMemory,
	//ClearStack,
	
	//Pushes values onto the stack (CPU stack)
	Push1, //X - 1 byte value
	Push4, //X - 4 byte value
	Push8, //X - 8 byte value
	PushX, //X - X byte value, X - value
	
	//Pops values from the stack
	Pop1, //X - 1 byte value
	Pop4, //X - 4 byte value
	Pop8, //X - 8 byte value
	PopX, //X - X byte value, X - value

	//Store value in dedicated memory
	Store1, //X - memory adress
	Store4, //X - memory adress
	Store8, //X - memory adress
	StoreX, //X - X value size, X - memory adress
	
	//Load value from memory
	Load1, //X - memory adress
	Load4, //X - memory adress
	Load8, //X - memory adress
	LoadX, //X - X value size, X - memory adress

	//Allocate space for a value
	Alloc1, //X - memory adress
	Alloc4, //X - memory adress
	Alloc8, //X - memory adress
	AllocX, //X - memory adress

	//Free a value at adress
	FREE, //X - memory adress

	//Add numbers on the stack together
	AddByte,
	AddInt,
	AddLong,
	AddFloat,
	AddDouble,

	//Subtract values on the stack
	//stack before:    	stack after
	// 2				0
	// 4				2
	//Operation is applied to the first
	//pushed value
	SubByte,
	SubInt,
	SubLong,
	SubFloat,
	SubDouble,
	
	//Multiply numbers on the stack
	MulByte,
	MulInt,
	MulLong,
	MulFloat,
	MulDouble,
	
	//Divide numbers on the stack
	//in the same order as subtraction
	DivByte,
	DivInt,
	DivLong,
	DivFloat,
	DivDouble,

	//Casting bytes to other data
	ByteToInt,
	ByteToLong,
	ByteToFloat,
	ByteToDouble,
	
	//Casting integers to other data
	IntToByte,
	IntToLong,
	IntToFloat,
	IntToDouble,

	//Casting floats to other data
	FloatToByte,//
	FloatToInt,//
	FloatToLong,//
	FloatToDouble,//

	//Casting doubles to other data
	DoubleToByte,//
	DoubleToInt,//
	DoubleToLong,//
	DoubleToFloat,//
	
	//Smaller than operators
	//Same order as with subtraction
	SmallerThanByte,
	SmallerThanInt,
	SmallerThanLong,
	SmallerThanFloat,
	SmallerThanDouble,

	//Larger than operators
	//Same order as subtraction
	BiggerThanByte,
	BiggerThanInt,
	BiggerThanLong,
	BiggerThanFloat,
	BiggerThanDouble,

	//Equality operators
	EqualByte,
	EqualInt,
	EqualLong,
	EqualFloat,
	EqualDouble,

	//All of the comparison operators return either
	// a 1 - true or 0 - false to the stack
	
	//If theres a 1 on the stack
	If,
	//If the isn't one
	IfNot
}SilentBytecode;
//Generate bytecode for the VM
vector* silentGenerateBytecode(silentProgram* program);
//Generate assembly for bytecode assembler
vector* silentGenerateAssembly(silentProgram* program);
//Generate library to link with other files
vector* silentGenerateLibrary(silentProgram* program);
//Write bytecode to file
void silentWriteBytecode(char* outFile, vector* outStream);
//Write assembly to file
void silentWriteAssembly(char* outFile, vector* outStream);
//Write library file
void silentWriteLibrary(char* outFile, vector* outStream);