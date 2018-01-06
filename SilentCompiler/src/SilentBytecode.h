#include "SilentParser.h"
vector* silentGenerateBytecode(silentProgram* program);
vector* silentGenerateAssembly(silentProgram* program);
vector* silentGenerateLibrary(silentProgram* program);
void silentWriteOutput(char* outFile, vector* outStream);