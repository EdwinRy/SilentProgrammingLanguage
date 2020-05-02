#include "silent_parser.h"
#include <stdlib.h>

Parser ParserInit(Tokenizer tokenizer)
{
	Parser parser = { 0 };
	parser.tokenizer = tokenizer;
	parser.root.type = PARSER_TYPE_GLOBAL;
	parser.root.global->node = &parser.root.global;
	parser.root.parent = NULL;
}

void Parse(Parser parser)
{

}

void ParserTest(Parser parser)
{

}

ParserNode ParseNamespace()
{

}