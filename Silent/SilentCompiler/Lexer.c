#include "Lexer.h"

void clearCodeBuffer(char* buffer, int *bufferPointer) 
{

	for (int i = 0; i <= *bufferPointer; i++) 
	{
		*(buffer + i) = 0;
	}

	bufferPointer = 0;

}

char addToken(Program *program, char *token, int *bufferPointer)
{
	if (bufferPointer == 0) { return -1; }
	token[*bufferPointer] = '\0';

	printf(token);

	if (!strcmp(token, "public")) 
	{
		*(program->tokens + program->tokensPointer) =  Public; 
		program->tokensPointer++;
	}

	else if (!strcmp(token, "private")) 
	{
		*(program->tokens + program->tokensPointer) = Private; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "protected")) 
	{
		*(program->tokens + program->tokensPointer) = Protected; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "static")) 
	{
		*(program->tokens + program->tokensPointer) = Static; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "namespace")) 
	{ 
		*(program->tokens + program->tokensPointer) = Namespace; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "using")) 
	{
		*(program->tokens + program->tokensPointer) = Using; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "class")) 
	{
		*(program->tokens + program->tokensPointer) = Class; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "method")) 
	{
		*(program->tokens + program->tokensPointer) = Method;
		program->tokensPointer++;
	}
	else if (!strcmp(token, "function")) 
	{
		*(program->tokens + program->tokensPointer) = Function; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "return")) 
	{
		*(program->tokens + program->tokensPointer) = Return; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "struct")) 
	{
		*(program->tokens + program->tokensPointer) = Struct; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "array")) 
	{
		*(program->tokens + program->tokensPointer) = Array; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "while")) 
	{
		*(program->tokens + program->tokensPointer) = While; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "for")) 
	{
		*(program->tokens + program->tokensPointer) = For; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "if")) 
	{
		*(program->tokens + program->tokensPointer) = If; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "elif")) 
	{
		*(program->tokens + program->tokensPointer) = Elif; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "else")) 
	{
		*(program->tokens + program->tokensPointer) = Else; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "new")) 
	{ 
		*(program->tokens + program->tokensPointer) = New; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "void")) 
	{
		*(program->tokens + program->tokensPointer) = Void; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "int")) 
	{
		*(program->tokens + program->tokensPointer) = Integer; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "float")) 
	{
		*(program->tokens + program->tokensPointer) = Float; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "string")) 
	{
		*(program->tokens + program->tokensPointer) = String; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "char")) 
	{
		*(program->tokens + program->tokensPointer) = Char; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "var")) 
	{
		*(program->tokens + program->tokensPointer) = Variable; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "==")) 
	{
		*(program->tokens + program->tokensPointer) = Equal; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "!=")) 
	{
		*(program->tokens + program->tokensPointer) = IsNot; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, ">=")) 
	{ 
		*(program->tokens + program->tokensPointer) = MoreThan; 
		program->tokensPointer++;
	}
	else if (!strcmp(token, "<=")) 
	{
		*(program->tokens + program->tokensPointer) = LessThan; 
		program->tokensPointer++;
	}
	else 
	{
		*(program->tokens + program->tokensPointer) = Value;
		program->tokensPointer++;
		program->values[program->valuePointer] =
			*(program->tokens + program->tokensPointer);
	}
	clearCodeBuffer(token, bufferPointer);

}

void AppendToBuffer(char* buffer, unsigned int *bufferPointer, char appendData) 
{
	buffer[*bufferPointer] = appendData;
	*bufferPointer++;
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

		if (!scanning)
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
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
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Fullstop;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '=')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Assign;
			program->tokensPointer++;
		}
		else if (*(program->source + i) == ',')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Comma;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '+')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Add;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '-')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Subtract;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '*')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Asterisk;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '//')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Divide;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == ';')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = Semicolon;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '[')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = OpenBracket;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == ']')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = CloseBracket;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '(')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = OpenParentheses;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == ')')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = CloseParentheses;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '{')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = OpenCurlyBracket;
			program->tokensPointer++;
		}

		else if (*(program->source + i) == '}')
		{
			addToken(program, CodeBuffer, &CodeBufferPointer);
			*(program->tokens + program->tokensPointer) = CloseCurlyBracket;
			program->tokensPointer++;
		}

		//if the value is not a token add it to the buffer
		else
		{
			*(CodeBuffer + CodeBufferPointer) = *(program->source + i);
			CodeBufferPointer++;
		}
	}
}

