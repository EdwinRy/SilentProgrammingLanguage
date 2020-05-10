#include "silent_parser.h"
#include <stdlib.h>

Parser ParserInit(Tokenizer tokenizer)
{
	Parser parser = { 0 };
	parser.tokenizer = tokenizer;
	parser.root.type = PARSER_TYPE_GLOBAL;
	parser.root.global = calloc(1, sizeof(ParserGlobal));
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

ParserNode ParseFunction()
{

}

ParserNode ParseWhileLoop()
{

}

ParserNode ParseForLoop()
{

}

ParserNode ParseType()
{

}

ParserNode ParseMethod()
{

}

ParserNode ParseStatement()
{

}

ParserNode ParseCall()
{

}

ParserNode ParseExpression()
{

}

ParserNode ParseMultiplication()
{

}

ParserNode ParseAddition()
{

}

ParserNode ParseCondition()
{

}

ParserNode ParseLogicalOp()
{

}

char ParseFactor(Parser parser)
{
	
	return 0;
}