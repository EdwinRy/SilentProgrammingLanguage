#include <stdlib.h>
#include "semantics.h"
#include "SilentC.h"


static SilentProgram* Program(ParseNode* program)
{

}



char GenerateSilentProgram(SilentCompileInfo* info)
{
    SilentProgram* program = Program(info->parser->root);
    if(program != NULL)
    {
        info->program = program;
        return 1;
    }
    else return 0;

}