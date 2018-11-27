#include "SilentTokenizer.hpp"
#define DEBUG 0
namespace Silent
{
    SilentTokenizer::SilentTokenizer()
    {
        this->tokens = std::vector<SilentToken>();
        this->line = 1;
    }

    std::string SilentTokenizer::TokenizeString(
        std::string source, uint64 &index
    )
    {
        std::string out;
        index += 1;
        while(source[index] != '\"')
        {
            if(source[index] == '\\')
            {
                index += 1;
                switch(source[index])
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
                        printf("Incorrect string formatting on line %i\n",
                            (int)line);
                    break;
                }
                index += 1;
                continue;
            }
            if(source[index] == '\n')
            {
                this->line += 1;
            }
            out += source[index];
            index += 1;
        }
        return out;
    }

    bool SilentTokenizer::Tokenize(std::string source)
    {
        //uint64 line = 1;
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
                        this->line++;
                    }
                    i++;
                }
                i++;
                continue;
            }

            SilentToken token;
            token.type = SilentTokenType::Unrecognised;
            token.value = "";
            token.line = this->line;

            switch(source[i])
            {
                case ';':
                    token.type = SilentTokenType::Semicolon;
                    token.value = ";";
                break;
                case ',':
                    token.type = SilentTokenType::Comma;
                    token.value = ",";
                break;
                case '=':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::Equal;
                        token.value = "==";
                    }
                    else
                    {
                        token.type = SilentTokenType::Assign;
                        token.value = "=";
                    }
                break;
                case '&':
                    token.type = SilentTokenType::And;
                    token.value = "&";
                break;
                case '|':
                    token.type = SilentTokenType::Or;
                    token.value = "|";
                break;
                case '^':
                    token.type = SilentTokenType::Xor;
                    token.value = "^";
                break;
                case '!':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::NotEqual;
                        token.value = "!=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Not;
                        token.value = "!";
                    }
                break;
                case '+':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::AddAssign;
                        token.value = "+=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Add;
                        token.value = "+";
                    }
                break;
                case '-':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::SubtractAssign;
                        token.value = "-=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Subtract;
                        token.value = "-";
                    }
                break;
                case '*':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::MultiplyAssign;
                        token.value = "*=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Multiply;
                        token.value = "*";
                    }
                break;
                case '/':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::DivideAssign;
                        token.value = "/=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Divide;
                        token.value = "/";
                    }
                break;
                case '<':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::SmallerOrEqual;
                        token.value = "<=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Smaller;
                        token.value = "<";
                    }
                break;
                case '>':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = SilentTokenType::LargerOrEqual;
                        token.value = ">=";
                    }
                    else
                    {
                        token.type = SilentTokenType::Larger;
                        token.value = ">";
                    }
                break;
                case '(':
                    token.type = SilentTokenType::OpenParam;
                    token.value = "(";
                break;
                case ')':
                    token.type = SilentTokenType::CloseParam;
                    token.value = ")";
                break;
                case '[':
                    token.type = SilentTokenType::OpenBracket;
                    token.value = "[";
                break;
                case ']':
                    token.type = SilentTokenType::CloseBracket;
                    token.value = "]";
                break;
                case '{':
                    token.type = SilentTokenType::OpenScope;
                    token.value = "{";
                break;
                case '}':
                    token.type = SilentTokenType::CloseScope;
                    token.value = "}";
                break;
                case '\"':
                    token.type = SilentTokenType::String;
                    token.value = this->TokenizeString(source, i);
                break;
                case ':':
                    if((source[++i] = ':'))
                    {
                        token.type = SilentTokenType::ScopeResolution;
                        token.value = "::";
                    }
                    else
                    {
                        i--;
                        token.type = SilentTokenType::Unrecognised;
                        token.value = ":";
                    }
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
                            token.type = SilentTokenType::Function;
                        }
                        else if(token.value == "if")
                        {
                            token.type = SilentTokenType::If;
                        }
                        else if(token.value == "ifn")
                        {
                            token.type = SilentTokenType::Ifn;
                        }
                        else if(token.value == "else")
                        {
                            token.type = SilentTokenType::Else;
                        }
                        else if(token.value == "while")
                        {
                            token.type = SilentTokenType::While;
                        }
                        else if(token.value == "struct")
                        {
                            token.type = SilentTokenType::Struct;
                        }
                        else if(token.value == "namespace")
                        {
                            token.type = SilentTokenType::Namespace;
                        }
                        else if(token.value == "class")
                        {
                            token.type = SilentTokenType::Class;
                        }
                        else if(token.value == "method")
                        {
                            token.type = SilentTokenType::Method;
                        }
                        else if(token.value == "new")
                        {
                            token.type = SilentTokenType::New;
                        }
                        else if(token.value == "delete")
                        {
                            token.type = SilentTokenType::Delete;
                        }
                        else if(token.value == "ref")
                        {
                            token.type = SilentTokenType::Reference;
                        }
                        else if(token.value == "return")
                        {
                            token.type = SilentTokenType::Return;
                        }
                        else if(token.value == "int8")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "uint8")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "int16")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "uint16")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "int32" || token.value == "int")
                        {
                            token.value = "int32";
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "uint32")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "int64" || token.value == "long")
                        {
                            token.value = "int64";
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "uint64")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "float32")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "float64")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else if(token.value == "string")
                        {
                            token.type = SilentTokenType::Primitive;
                        }
                        else
                        {
                            token.type = SilentTokenType::Identifier;
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
                        token.type = SilentTokenType::Number;
                    }
                    else
                    {
                        continue;
                    }
                break;
            }
            #if DEBUG
            std::cout << token.line << (": " + token.value + "\n").data();
            #endif
            this->tokens.push_back(token);
        }
        return true;
    }

    std::vector<SilentToken> SilentTokenizer::GetTokens(){return this->tokens;}
}