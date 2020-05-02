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
    return t;
}

Token GetToken(Tokenizer* tokenizer)
{
    return tokenizer->tokens.tokens[tokenizer->tokens.ptr];
}

void NextToken(Tokenizer* tokenizer)
{
    if(tokenizer->tokens.ptr < tokenizer->tokens.count)
        tokenizer->tokens.ptr++;
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
    printf("Lexer Error: Unexpected token on line %i", tokenizer->line);
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

char* ParseStringValue(char* source, uint64* i, uint64* line)
{
    // skip "
    (*i)++;
    char* buff = malloc(ARR_BLOCK_SIZE);
    if (buff == NULL) exit(-1);
    int buffPtr = 0;
    int buffAlloc = ARR_BLOCK_SIZE;
    while (source[*i] != '\"')
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += ARR_BLOCK_SIZE;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }
        if (source[*i] == '\\')
        {
            (*i)++;
            switch (source[*i])
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
                printf("Incorrect string formatting on line %i\n",
                    (int)*line);
                break;
            }
            (*i)++;
            continue;
        }
        if (source[*i] == '\n')
        {
            (*line)++;
        }

        buff[buffPtr++] = source[(*i)++];
    }
    (*i)++;
    return buff;
}

inline char IsIdstart(char c) 
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
