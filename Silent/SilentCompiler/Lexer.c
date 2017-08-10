#include "Lexer.h"

char determineToken(char* token) 
{
	if (!strcmp(token, "public")) { return Public; }
	else if (!strcmp(token, "private")) { return Private; }
	else if (!strcmp(token, "protected")) { return Protected; }
	else if (!strcmp(token, "static")) { return Static; }
	else if (!strcmp(token, "namespace")) { return Namespace; }
	else if (!strcmp(token, "using")) { return Using; }
	else if (!strcmp(token, "class")) { return Class; }
	else if (!strcmp(token, "method")) { return Method; }
	else if (!strcmp(token, "function")) { return Function; }
	else if (!strcmp(token, "return")) { return Return; }
	else if (!strcmp(token, "struct")) { return Struct; }
	else if (!strcmp(token, "array")) { return Array; }
	else if (!strcmp(token, "while")) { return While; }
	else if (!strcmp(token, "for")) { return For; }
	else if (!strcmp(token, "if")) { return If; }
	else if (!strcmp(token, "elif")) { return Elif; }
	else if (!strcmp(token, "else")) { return Else; }
	else if (!strcmp(token, "new")) { return New; }
	else if (!strcmp(token, "void")) { return Void; }
	else if (!strcmp(token, "int")) { return Integer; }
	else if (!strcmp(token, "float")) { return Float; }
	else if (!strcmp(token, "string")) { return String; }
	else if (!strcmp(token, "char")) { return Char; }
	else if (!strcmp(token, "var")) { return Variable; }
	else if (!strcmp(token, "==")) { return Equal; }
	else if (!strcmp(token, "!=")) { return IsNot; }
	else if (!strcmp(token, ">=")) { return MoreThan; }
	else if (!strcmp(token, "<=")) { return LessThan; }
	else { return Value;}
}

void clearCodeBuffer(char* buffer, unsigned int *bufferPointer) 
{

	for (int i = 0; i <= *bufferPointer; i++) 
	{
		buffer[i] = 0;
	}

	*bufferPointer = 0;

}


void Tokenize(Program *program)
{
	//allocate scanning variables
	char* CodeBuffer = malloc(50);
	unsigned int CodeBufferPointer = 0;
	char scanning = 1;

	//main scanning loop
	for (unsigned long i = 0; i <= program->sourceLength; i++)
	{
		//printf("%i", *(program->source + i));

		if (!scanning)
		{
			if (CodeBufferPointer == 0) { continue; }
			char token = determineToken(CodeBuffer);
			if (token == Value)
			{
				char* value = malloc(CodeBufferPointer + 1);
			}
			program->tokens[program->tokensPointer] = token;
			clearCodeBuffer(CodeBuffer, &CodeBufferPointer);
			scanning = 1;
		}

		//check for spaces
		else if (*(program->source + i) == ' ') { scanning = 0; continue; }
		else if (*(program->source + i) == '\t') { scanning = 0; continue; }
		else if (*(program->source + i) == '\n') { scanning = 0; continue; }
		else if (*(program->source + i) == '\r') { scanning = 0; continue; }
		else if (*(program->source + i) == '\f') { scanning = 0; continue; }
		else if (*(program->source + i) == '\v') { scanning = 0; continue; }

		else if (*(program->source + i) == '.')
		{
			program->tokens[program->tokensPointer] = Fullstop;
		}

		else if (*(program->source + i) == '=')
		{
			program->tokens[program->tokensPointer] = Assign;
		}
		else if (*(program->source + i) == ',')
		{
			program->tokens[program->tokensPointer] = Comma;
		}

		else if (*(program->source + i) == '+')
		{
			program->tokens[program->tokensPointer] = Add;
		}

		else if (*(program->source + i) == '-')
		{
			program->tokens[program->tokensPointer] = Subtract;
		}

		else if (*(program->source + i) == '*')
		{
			program->tokens[program->tokensPointer] = Asterisk;
		}

		else if (*(program->source + i) == '//')
		{
			program->tokens[program->tokensPointer] = Divide;
		}

		else if (*(program->source + i) == ';')
		{
			program->tokens[program->tokensPointer] = Semicolon;
		}

		else if (*(program->source + i) == '[')
		{
			program->tokens[program->tokensPointer] = OpenBracket;
		}

		else if (*(program->source + i) == ']')
		{
			program->tokens[program->tokensPointer] = CloseBracket;
		}

		else if (*(program->source + i) == '(')
		{
			program->tokens[program->tokensPointer] = OpenParentheses;
		}

		else if (*(program->source + i) == ')')
		{
			program->tokens[program->tokensPointer] = CloseParentheses;
		}

		else if (*(program->source + i) == '{')
		{
			program->tokens[program->tokensPointer] = OpenCurlyBracket;
		}

		else if (*(program->source + i) == '}')
		{
			program->tokens[program->tokensPointer] = CloseCurlyBracket;
		}

		//if the value is not a token add it to the buffer
		else
		{
			CodeBuffer[CodeBufferPointer] = program->source[i];
			CodeBufferPointer++;
		}
	}

}

