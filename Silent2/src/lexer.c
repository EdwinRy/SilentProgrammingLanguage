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



//TokenList* tokenlist_init()
//{
//    TokenList* tokenList = calloc(1, sizeof(TokenList));
//    tokenList->count = 0;
//    tokenList->allocated = 10;
//    tokenList->tokens = calloc(10, sizeof(Token));
//}
//
//void tokenlist_add(TokenList* list, Token* token)
//{
//    if (list->allocated <= list->count)
//    {
//        list->allocated += 1 + list->allocated * 0.1;
//        list->tokens = realloc(
//            list->tokens, 
//            list->allocated * sizeof(Token));
//    }
//
//    memcpy(list->tokens + list->count, token, sizeof(Token));
//    list->count++;
//
//
//}
//
//void tokenlist_resize(TokenList* list)
//{
//
//}
//
//
//TokenList* silent_tokenize(char* src)
//{
//    TokenList* tokenList = calloc(1, sizeof(TokenList));
//    tokenList->count = 0;
//    tokenList->tokens = calloc(TOKEN_LIST_CHUNK, sizeof(Token));
//    uint64 tokensAllocated = TOKEN_LIST_CHUNK;
//
//    uint64 srcLen = strlen(src);
//    uint64 line = 0;
//
//    for(uint64 i = 0; i < srcLen; i++)
//    {
//        //Skip next line
//        if(src[i] == '\n')
//        {
//            line++;
//            continue;
//        }
//        //One line comment
//        if(src[i] == '/' && src[i+1] == '/')
//        {
//            while(src[i] != '\n')
//            {
//                i++;
//            }
//            i--;
//            continue;
//        }
//        //Multi-line comment
//        if(src[i] == '/' && src[i+1] == '*')
//        {
//            while(!(src[i] == '*' && src[i+1] == '/'))
//            {
//                if(src[i] == '\n')
//                {
//                    line++;
//                }
//                i++;
//            }
//            i++;
//            continue;
//        }
//
//        Token token = {0};
//        token.type = tkn_id;
//        token.line = line;
//
//        switch(src[i])
//        {
//            case ';':
//                token.type = tkn_semicolon;
//            break;
//
//            case ',':
//                token.type = tkn_comma;
//            break;
//
//            case '.':
//                token.type = tkn_fullstop;
//            break;
//
//            case '=':
//                if(src[i+1] == '=')
//                {
//                    i++;
//                    token.type = tkn_equal;
//                }
//                else
//                {
//                    token.type = tkn_assign;
//                }
//            break;
//
//            case '&':
//                if(src[i+1] == '&')
//                {
//                    i++;
//                    token.type = tkn_cond_and;
//                }
//                else
//                {
//                    token.type = tkn_and; 
//                }
//            break;
//
//            default:
//                
//            break;
//        }
//
//        if(tokenList->count == tokensAllocated)
//        {
//            tokensAllocated += TOKEN_LIST_CHUNK;
//            Token* temp = (Token*)realloc(tokenList->tokens, tokensAllocated);
//            if(temp == NULL)
//            {
//                // TODO: Handle a catastrophe
//            }
//            else tokenList->tokens = temp;
//        }
//    }
//}