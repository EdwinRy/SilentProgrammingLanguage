#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define TokensAlloc 1024
#define idBuffAlloc 1024

static void SkipWhitespaces(TokenizerInfo* info)
{
    while(isspace(info->src[info->srcPtr]))
        info->srcPtr++;
}

static void SkipComments(TokenizerInfo* info)
{
    SkipWhitespaces(info);
    char* src = info->src;
    uint64 ptr = info->srcPtr;
    if(src[ptr] == '/')
    {
        switch(src[ptr + 1])
        {
            // Single-line comment
            case '/':
                ptr += 2;
                while(src[ptr] != '\n')
                {
                    if(src[ptr] == '\0') break;
                    ptr++;
                }
            break;

            case '*':
                ptr += 2;
                while(src[ptr] != '\0')
                {
                    if(src[ptr] == '*' && src[ptr + 1] == '/')
                    {
                        ptr += 2;
                        break;
                    }
                    ptr++;
                }
            break;
        }
    }
    info->srcPtr = ptr;
}

static void SkipToSrc(TokenizerInfo* info)
{
    SkipComments(info);
    SkipWhitespaces(info);
}

static inline char IsIdStart(char c) 
{
    if (isalpha(c)) return 1;
    else if (c == '_') return 1;
    return 0;
}

static inline char IsNumStart(char c) 
{
    if (isdigit(c)) return 1;
    else if (c == '.') return 1;
    return 0;
}

static char* ParseIdentifier(TokenizerInfo* info)
{
    char* buff = malloc(idBuffAlloc);
    int buffPtr = 0;
    int buffAlloc = idBuffAlloc;

    char* src = info->src;
    uint64 ptr = info->srcPtr;

    while(IsIdStart(src[ptr]) || isdigit(src[ptr]))
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += idBuffAlloc;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }

        buff[buffPtr++] = src[ptr++];
    }
    info->srcPtr = ptr;
    char* id = realloc(buff, buffPtr);
    id[buffPtr] = '\0';
    return id;
}

static char* ParseNumber(TokenizerInfo* info)
{
    char* buff = malloc(idBuffAlloc);
    int buffPtr = 0;
    int buffAlloc = idBuffAlloc;

    char* src = info->src;
    uint64 ptr = info->srcPtr;

    char dotHit = 0;
    char parsing = 1;

    while(parsing)
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += idBuffAlloc;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }

        if(isdigit(src[ptr]))
        {
            buff[buffPtr++] = src[ptr++];
        }

        else if(src[ptr] == '.')
        {
            // if(dotHit == 1)
            // {
            //     printf("Unexpected '.' on line %lli skipped\n", 
            //         tokenizer->line);
            //     tokenizer->srcptr++;
            //     continue;
            // }
            buff[buffPtr++] = '.';
            ptr++;
            dotHit = 1;
        }

        else parsing = 0;
    }
    info->srcPtr = ptr;
    char* num = realloc(buff, buffPtr);
    num[buffPtr] = '\0';
    return num;
}

static char* ParseStringValue(TokenizerInfo* info)
{
    char* src = info->src;
    uint64 ptr = info->srcPtr;

    ptr++;
    char* buff = malloc(idBuffAlloc);
    if (buff == NULL) exit(-1);
    int buffPtr = 0;
    int buffAlloc = idBuffAlloc;

    while (src[ptr] != '\"')
    {
        if (buffPtr == buffAlloc)
        {
            buffAlloc += idBuffAlloc;
            char* temp = realloc(buff, buffAlloc);
            if(temp == NULL) exit(-1);
            buff = temp;
        }
        if (src[ptr] == '\\')
        {
            ptr++;
            switch (src[ptr])
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
                    // printf("Incorrect string formatting on line %lli\n",
                    //     tokenizer->line);
                break;
            }
            ptr++;
            continue;
        }
        // if (src[ptr] == '\n')
        //     tokenizer->line++;

        buff[buffPtr++] = src[ptr++];
    }
    ptr++;
    info->srcPtr = ptr;
    char* str = realloc(buff, buffPtr);
    str[buffPtr] = '\0';
    return str;
}

static TokenType GetStrTokenType(char* str)
{
    switch (str[0])
    {
        case 't':
            if (strcmp(str, "type") == 0) return tkn_Type;
        break;

        case 'i':
            if (strcmp(str, "if") == 0) return tkn_If;
            else if (strcmp(str, "ifn") == 0) return tkn_IfNot;
            else if (strcmp(str, "int8") == 0) return tkn_I8;
        break;

        case 'e':
            if (strcmp(str, "else") == 0) return tkn_Else;
        break;

        case 'f':
            if (strcmp(str, "for") == 0) return tkn_For;
        break;

        case 'w':
            if (strcmp(str, "while") == 0) return tkn_While;
        break;

        case 'r':
            if (strcmp(str, "return") == 0) return tkn_Return;
        break;

        case 'n':
            if (strcmp(str, "namespace") == 0) return tkn_Namespace;
        break;

        default:
            return tkn_Identifier;
        break;
    }
    return tkn_Identifier;
}

static Token GetToken(TokenizerInfo* info)
{
    Token token = {0};
    char* src = info->src;
    uint64 ptr = info->srcPtr;
    switch(src[ptr])
    {
        case '\0':
            token.type = tkn_EndOfFile;
        break;

        case ';':
            token.type = tkn_Semicolon;
            ptr++;
        break;

        case ',':
            token.type = tkn_Comma;
            ptr++;
        break;

        case '.':
            token.type = tkn_Fullstop;
            ptr++;
        break;

        case '=':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_Equal;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Assign;
                ptr++;
            }
            break;

        case '&':
            if (src[ptr + 1] == '&')
            {
                token.type = tkn_ConditionalAnd;
                ptr += 2;
            }
            else
            {
                token.type = tkn_And;
                ptr++;
            }
            break;

        case '|':
            if (src[ptr + 1] == '|')
            {
                token.type = tkn_ConditionalOr;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Or;
                ptr++;
            }
        break;

        case '^':
            token.type = tkn_Xor;
        break;

        case '!':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_NotEqual;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Not;
                ptr++;
            }
        break;

        case '+':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_AddAssign;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Add;
                ptr++;
            }
        break;

        case '-':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_SubtractAssign;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Subtract;
                ptr++;
            }
        break;

        case '*':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_MultiplyAssign;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Multiply;
                ptr++;
            }
        break;

        case '/':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_DivideAssign;
                ptr += 2;
            }
            else
            {
                token.type = tkn_Divide;
                ptr++;
            }
        break;

        case '<':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_LessThanOrEqualTo;
                ptr += 2;
            }
            else
            {
                token.type = tkn_LessThan;
                ptr++;
            }
        break;

        case '>':
            if (src[ptr + 1] == '=')
            {
                token.type = tkn_MoreThanOrEqualTo;
                ptr += 2;
            }
            else
            {
                token.type = tkn_MoreThan;
                ptr++;
            }
        break;

        case '(':
            token.type = tkn_OpenParam;
            ptr++;
        break;

        case ')':
            token.type = tkn_CloseParam;
            ptr++;
        break;

        case '[':
            token.type = tkn_OpenBracket;
            ptr++;
        break;

        case ']':
            token.type = tkn_CloseBracket;
            ptr++;
        break;

        case '{':
            token.type = tkn_OpenScope;
            ptr++;
        break;

        case '}':
            token.type = tkn_CloseScope;
            ptr++;
        break;

        case ':':
            token.type = tkn_Colon;
            ptr++;
        break;

        case '\"':
            token.type = tkn_StringValue;
            token.id = ParseStringValue(info);
        break;

        default:
            info->srcPtr = ptr;
            if (IsIdStart(src[ptr]))
            {
                token.id = ParseIdentifier(info);
                token.type = GetStrTokenType(token.id);
                if(token.type == tkn_Identifier)
                    SymbolTableLinkInsert(info->symLink, &token.id);
                else free(token.id);
            }
            else if (IsNumStart(src[ptr]))
            {
                token.id = ParseNumber(info);
                token.type = tkn_Number;
                SymbolTableLinkInsert(info->symLink, &token.id);
            }
            ptr = info->srcPtr;
        break;
    }
    info->srcPtr = ptr;
    return token;
}

static void AppendToken(TokenizerInfo* info, Token tkn)
{
    if(info->count == info->allocated)
    {
        Token* temp = realloc(info->tokens, info->allocated + TokensAlloc);
        if(temp == NULL) return; // TODO: Handle Error
        info->tokens = temp;
    }
    info->tokens[info->count++] = tkn;
}

TokenizerInfo Tokenize(char* source){
    TokenizerInfo info = {0};
    info.tokens = calloc(TokensAlloc, sizeof(Token));
    info.src = source;
    info.symLink = SymbolTableLinkInit();

    Token tkn;
    do
    {
        SkipToSrc(&info);
        tkn = GetToken(&info);
        AppendToken(&info, tkn);
    }while(tkn.type != tkn_EndOfFile);

    info.success = 1;
    return info;
}
