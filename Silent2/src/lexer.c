#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "silent_lexer.h"

//#define TOKEN_LIST_CHUNK 20

typedef unsigned long long uint64;

#define ARR_BLOCK_SIZE 1024

Tokenizer TokenizerInit(char* src)
{
    Tokenizer t = {0};
    t.src = src;
    t.tokens = TokenListInit();
    return t;
}

TokenList TokenListInit()
{
    TokenList tl = {0};
    tl.tokens = calloc(sizeof(Token), ARR_BLOCK_SIZE);
    tl.allocated = ARR_BLOCK_SIZE;
}

Token GetToken(Tokenizer* tokenizer)
{
    return tokenizer->tokens.tokens[tokenizer->tokens.ptr];
}

void NextToken(Tokenizer* tokenizer)
{
    // Allocate more space if needed
    if(tokenizer->tokens.ptr + 1 > tokenizer->tokens.allocated)
    {
        Token* temp = realloc(
            tokenizer->tokens.tokens, 
            tokenizer->tokens.allocated + ARR_BLOCK_SIZE);
        
        // If reallocation failed
        if(temp == NULL)
        {
            printf("CRITICAL ERROR: FAILED TO REALLOC MEMORY\n");
            exit(-1);
        }

        tokenizer->tokens.tokens = temp;
        tokenizer->tokens.allocated += ARR_BLOCK_SIZE;
    }

    // Fetch another token
    
}

char IsTokenType(Tokenizer* tokenizer, TokenType type)
{
    TokenType tknType = GetToken(tokenizer).type;
    switch (tknType)
    {
    default:
        return tknType == type;
    }
}

char AcceptToken(Tokenizer* tokenizer, TokenType type)
{
    if (GetToken(tokenizer).type == type)
    {
        NextToken(tokenizer);
        return 1;
    }
    else return 0;
}

char ExpectToken(Tokenizer* tokenizer, TokenType type)
{
    if (AcceptToken(tokenizer, type) == 1)
        return 1;
    printf("Lexer Error: Unexpected token on line %lli", tokenizer->line);
    return 0;
}

TokenType GetTokenType(char* str)
{
    switch (str[0])
    {
        case 't':
            if (strcmp(str, "type")) return tkn_Type;
        break;

        case 'i':
            if (strcmp(str, "if")) return tkn_If;
            else if (strcmp(str, "ifn")) return tkn_IfNot;
            else if (strcmp(str, "int8")) return tkn_I8;
        break;

        case 'e':
            if (strcmp(str, "else")) return tkn_Else;
        break;

        case 'f':
            if (strcmp(str, "for")) return tkn_For;
        break;

        case 'w':
            if (strcmp(str, "while")) return tkn_While;
        break;

        case 'r':
            if (strcmp(str, "return")) return tkn_Return;
        break;

        case 'n':
            if (strcmp(str, "namespace")) return tkn_Namespace;
        break;

        default:
            return tkn_Identifier;
        break;
    }
    return tkn_Identifier;
}

void LoadToken(Tokenizer* tokenizer)
{
    // Skip comments
    char* src = tokenizer->src;
    while(SkipComment(src, &tokenizer->srcptr, &tokenizer->line));

    Token token;
    switch(src[tokenizer->srcptr])
    {
        case ';':
            token.type = tkn_Semicolon;
        break;

        case ',':
            token.type = tkn_Comma;
        break;

        case '.':
            token.type = tkn_Fullstop;
        break;

        case '=':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;
                token.type = tkn_Equal;
            }
            else
            {
                token.type = tkn_Assign;
            }
            break;

        case '&':
            if (src[tokenizer->srcptr + 1] == '&')
            {
                tokenizer->srcptr++;;
                token.type = tkn_ConditionalAnd;
            }
            else
            {
                token.type = tkn_And;
            }
            break;

        case '|':
            if (src[tokenizer->srcptr + 1] == '|')
            {
                tokenizer->srcptr++;;
                token.type = tkn_ConditionalOr;
            }
            else
            {
                token.type = tkn_Or;
            }
        break;

        case '^':
            token.type = tkn_Xor;
        break;

        case '!':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_NotEqual;
            }
            else
            {
                token.type = tkn_Not;
            }
        break;

        case '+':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_AddAssign;
            }
            else
            {
                token.type = tkn_Add;
            }
        break;

        case '-':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_SubtractAssign;
            }
            else
            {
                token.type = tkn_Subtract;
            }
        break;

        case '*':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_MultiplyAssign;
            }
            else
            {
                token.type = tkn_Multiply;
            }
        break;

        case '/':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_DivideAssign;
            }
            else
            {
                token.type = tkn_Divide;
            }
        break;

        case '<':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_LessThanOrEqualTo;
            }
            else
            {
                token.type = tkn_LessThan;
            }
        break;

        case '>':
            if (src[tokenizer->srcptr + 1] == '=')
            {
                tokenizer->srcptr++;;
                token.type = tkn_MoreThanOrEqualTo;
            }
            else
            {
                token.type = tkn_MoreThan;
            }
        break;

        case '(':
            token.type = tkn_OpenParam;
        break;

        case ')':
            token.type = tkn_CloseParam;
        break;

        case '[':
            token.type = tkn_OpenBracket;
        break;

        case ']':
            token.type = tkn_CloseBracket;
        break;

        case '{':
            token.type = tkn_OpenScope;
        break;

        case '}':
            token.type = tkn_CloseScope;
        break;

        case ':':
            token.type = tkn_Colon;
        break;

        case '\"':
            token.type = tkn_StringValue;
            token.val = ParseStringValue(tokenizer);
        break;

        default:
            if (IsIdStart(src[tokenizer->srcptr]))
            {
                token.val = ParseIdentifier(tokenizer);
                token.type = GetTokenType(token.val);
            }
            else if (IsNumStart(src[tokenizer->srcptr]))
            {
                
            }
        break;
    }
}

void SkipWhitespaces(Tokenizer* tokenizer)
{
    while(isspace(tokenizer->src[tokenizer->srcptr]))
        tokenizer->srcptr++;
}

char SkipComment(char* src, unsigned long long* i, unsigned long long* line)
{
    if (src[*i] == '/')
    {
        switch (src[(*i) + 1])
        {
            // Skip single-line comment
            case '/':
                (*i) += 2;
                while (src[*i] != '\0')
                {
                    if (src[*i] == '\n')
                    {
                        (*line)++;
                        (*i)++;
                    }
                    (*i)++;
                }
                return 1;
            break;

            // Skip multi-line comment
            case '*':
                (*i) += 2;
                while (src[*i] != '\0')
                {
                    if (src[*i] == '\n') (*line)++;
                    if (src[*i] == '*' && src[(*i) + 1] == '/')
                    {
                        (*i) += 2;
                        break;
                    }
                    (*i)++;
                }
                return 1;
            break;
        }
    }
    return 0;
}

char* ParseIdentifier(Tokenizer* tokenizer)
{
    char* buff = malloc(ARR_BLOCK_SIZE);
    int buffPtr = 0;
    int buffAlloc = ARR_BLOCK_SIZE;

    while(IsIdStart(tokenizer->src[tokenizer->srcptr]) || 
            isdigit(tokenizer->src[tokenizer->srcptr]))
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += ARR_BLOCK_SIZE;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }

        buff[buffPtr++] = tokenizer->src[tokenizer->srcptr++];
    }
    
    char* id = realloc(buff, buffPtr);
    id[buffPtr] = '\0';
    return id;
}

char* ParseNumber(Tokenizer* tokenizer)
{
    char* buff = malloc(ARR_BLOCK_SIZE);
    int buffPtr = 0;
    int buffAlloc = ARR_BLOCK_SIZE;

    char dotHit = 0;
    char parsing = 1;

    while(parsing)
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += ARR_BLOCK_SIZE;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }

        if(isdigit(tokenizer->src[tokenizer->srcptr]))
        {
            buff[buffPtr++] = tokenizer->src[tokenizer->srcptr++];
        }

        else if(tokenizer->src[tokenizer->srcptr] == '.')
        {
            if(dotHit == 1)
            {
                printf("Unexpected '.' on line %lli skipped\n", 
                    tokenizer->line);
                tokenizer->srcptr++;
                continue;
            }
            buff[buffPtr++] = '.';
            tokenizer->srcptr++;
            dotHit = 1;
        }

        else parsing = 0;
    }

    char* num = realloc(buff, buffPtr);
    num[buffPtr] = '\0';
    return num;
}

char* ParseStringValue(Tokenizer* tokenizer)
{
    tokenizer->srcptr++;
    char* buff = malloc(ARR_BLOCK_SIZE);
    if (buff == NULL) exit(-1);
    int buffPtr = 0;
    int buffAlloc = ARR_BLOCK_SIZE;

    while (tokenizer->src[tokenizer->srcptr] != '\"')
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += ARR_BLOCK_SIZE;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }
        if (tokenizer->src[tokenizer->srcptr] == '\\')
        {
            tokenizer->srcptr++;
            switch (tokenizer->src[tokenizer->srcptr])
            {
                case 'n':
                    buff[buffPtr++] = '\n';
                break;
                case 't':
                    buff[buffPtr++] = '\t';
                    break;
                case '\"':
                    buff[buffPtr++] = '\"';
                    break;
                case '\\':
                    buff[buffPtr++] = '\\';
                    break;
                case 'v':
                    buff[buffPtr++] = '\v';
                    break;
                case 'r':
                    buff[buffPtr++] = '\r';
                    break;
                case 'f':
                    buff[buffPtr++] = '\f';
                    break;
                case '0':
                    buff[buffPtr++] = '\0';
                break;

                default:
                    printf("Incorrect string formatting on line %lli\n",
                        tokenizer->line);
                break;
            }
            tokenizer->srcptr++;
            continue;
        }
        if (tokenizer->src[tokenizer->srcptr] == '\n')
            tokenizer->line++;

        buff[buffPtr++] = tokenizer->src[tokenizer->srcptr++];
    }
    tokenizer->srcptr++;

    char* str = realloc(buff, buffPtr);
    str[buffPtr] = '\0';
    return str;
}

inline char IsIdStart(char c) 
{
    if (isalpha(c)) return 1;
    else if (c == '_') return 1;
    return 0;
}

inline char IsNumStart(char c) 
{
    if (isdigit(c)) return 1;
    else if (c == '.') return 1;
    return 0;
}
