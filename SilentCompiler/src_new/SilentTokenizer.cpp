#include "SilentTokenizer.hpp"
using namespace Silent;

typedef unsigned long long uint64;

std::string tokenizeString(std::string source, uint64* line, uint64* index)
{
    std::string out;
    *index += 1;
    while(source[*index] != '\"')
    {
        if(source[*index] == '\\')
        {
            *index += 1;
            switch(source[*index])
            {
                case 'n':
                    out += '\n';
                break;
                case 't':
                    out += '\t';
                break;
                case '\"':
                    out += '\"';
                break;
                case '\\':
                    out += '\\';
                break;
                case 'v':
                    out += '\v';
                break;
                case 'r':
                    out += '\r';
                break;
                case 'f':
                    out += '\f';
                break;
                case '0':
                    out += '\0';
                break;
                default:
                    printf("Incorrect string formatting on line %i\n",line);
                break;
            }
            *index += 1;
            continue;
        }
        if(source[*index] == '\n')
        {
            *line += 1;
        }
        out += source[*index];
        *index += 1;
    }
    return out;
}

std::vector<SilentToken>* Silent::SilentTokenize(std::string source)
{
    std::vector<SilentToken>* tokens = new std::vector<SilentToken>();
    uint64 line = 1;
    for(uint64 i = 0; i < source.length(); i++)
    {
        //Skip next line
        if(source[i] == '\n')
        {
            line++;
            continue;
        }
        //One line comment
        if(source[i] == '/' && source[i+1] == '/')
        {
            while(source[i] != '\n')
            {
                i++;
            }
            i--;
            continue;
        }
        //Multi-line comment
        if(source[i] == '/' && source[i+1] == '*')
        {
            while(!(source[i] == '*' && source[i+1] == '/'))
            {
                if(source[i] == '\n')
                {
                    line++;
                }
                i++;
            }
            i++;
            continue;
        }

        SilentToken token;
        token.type = SilentTokenType::unrecognised;
        token.value = "";
        token.line = line;

        switch(source[i])
        {
            case ';':
                token.type = SilentTokenType::symbol;
                token.value = ";";
            break;
            case ',':
                token.type = SilentTokenType::symbol;
                token.value = ",";
            break;
            case '=':
                token.type = SilentTokenType::expressionSymbol;
                token.value = "=";
            break;
            case '+':
                token.type = SilentTokenType::expressionSymbol;
                token.value = "+";
            break;
            case '-':
                token.type = SilentTokenType::expressionSymbol;
                token.value = "-";
            break;
            case '*':
                token.type = SilentTokenType::expressionSymbol;
                token.value = "*";
            break;
            case '/':
                token.type = SilentTokenType::expressionSymbol;
                token.value = "/";
            break;
            case '(':
                token.type = SilentTokenType::expressionSymbol;
                token.value = "(";
            break;
            case ')':
                token.type = SilentTokenType::expressionSymbol;
                token.value = ")";
            break;
            case '{':
                token.type = SilentTokenType::symbol;
                token.value = "{";
            break;
            case '}':
                token.type = SilentTokenType::symbol;
                token.value = "}";
            break;
            case '\"':
                token.type = SilentTokenType::stringValue;
                token.value = tokenizeString(source, &line, &i);
            break;
            default:
                if(isalpha(source[i]))
                {
                    while(isalpha(source[i]) || isdigit(source[i]))
                    {
                        token.value += source[i];
                        i++;
                    }
                    i--;
                    if(token.value == "func")
                    {
                        token.type = SilentTokenType::keyword;
                    }
                    else if(token.value == "struct")
                    {
                        token.type = SilentTokenType::keyword;
                    }
                    else if(token.value == "return")
                    {
                        token.type = SilentTokenType::keyword;
                    }
                    else if(token.value == "int8")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "uint8")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "int16")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "uint16")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "int32")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "uint32")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "int64")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "uint64")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "float32")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "float64")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else if(token.value == "string")
                    {
                        token.type = SilentTokenType::primitive;
                    }
                    else
                    {
                        token.type = SilentTokenType::identifier;
                    }
                }
                else if(isdigit(source[i]))
                {
                    while(isdigit(source[i]) || source[i] == '.')
                    {
                        token.value += source[i];
                        i++;
                    }
                    i--;
                    token.type = SilentTokenType::numericalValue;
                }
                else
                {
                    continue;
                }
            break;
        }
        std::cout << token.line << ": " + token.value + "\n";
        //printf("%i: %s\n",token.line, token.value.data());
        tokens->push_back(token);
    }
    return tokens;
}