#include "Lexer.h"
#include "Compiler.h"

void Tokenize(Program *program)
{
	char* CodeBuffer = malloc(50);
	unsigned int CodeBufferPointer = 0;

	for (unsigned long i = 0; i <= program->sourceLength; i++) {

	}

}

char determineToken(char* token) 
{
	if (strcmp(token, "public")) { return Public; }
	else if (strcmp(token, "private")) { return Private; }
	else if (strcmp(token, "protected")) { return Protected; }
	else if (strcmp(token, "static")) { return Static; }

	else if (strcmp(token, "namespace")) { return Namespace; }
	else if (strcmp(token, "using")) { return Using; }
	else if (strcmp(token, "class")) { return Class; }
	else if (strcmp(token, ".")) { return Fullstop; }

	else if (strcmp(token, "method")) { return Method; }
	else if (strcmp(token, "function")) { return Function; }

	else if (strcmp(token, "return")) { return Return; }
	else if (strcmp(token, "struct")) { return Struct; }
	else if (strcmp(token, "array")) { return Array; }

	else if (strcmp(token, "while")) { return While; }
	else if (strcmp(token, "for")) { return For; }

	else if (strcmp(token, "if")) { return If; }
	else if (strcmp(token, "elif")) { return Elif; }
	else if (strcmp(token, "else")) { return Else; }

	else if (strcmp(token, "new")) { return New; }
	else if (strcmp(token, "=")) { return Assign; }
	else if (strcmp(token, "void")) { return Void; }
	else if (strcmp(token, "int")) { return Integer; }
	else if (strcmp(token, "float")) { return Float; }
	else if (strcmp(token, "string")) { return String; }
	else if (strcmp(token, "char")) { return Char; }
	else if (strcmp(token, "var")) { return Variable; }
	else if (strcmp(token, ",")) { return Comma; }

	else if (strcmp(token, "+")) { return Add; }
	else if (strcmp(token, "-")) { return Subtract; }
	else if (strcmp(token, "*")) { return Asterisk; }
	else if (strcmp(token, "//")) { return Divide; }

	else if (strcmp(token, "==")) { return Equal; }
	else if (strcmp(token, "!=")) { return IsNot; }
	else if (strcmp(token, ">=")) { return MoreThan; }
	else if (strcmp(token, "<=")) { return LessThan; }

	else if (strcmp(token, ";")) { return Semicolon; }
	else if (strcmp(token, "[")) { return OpenBracket; }
	else if (strcmp(token, "]")) { return CloseBracket; }
	else if (strcmp(token, "(")) { return OpenParentheses; }
	else if (strcmp(token, ")")) { return CloseParentheses; }
	else if (strcmp(token, "{")) { return OpenCurlyBracket; }
	else if (strcmp(token, "}")) { return CloseCurlyBracket; }

	else { return Value;}

}

