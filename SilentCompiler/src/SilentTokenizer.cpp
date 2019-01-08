#include "SilentTokenizer.hpp"
#define DEBUG 0
namespace Silent
{
    Tokenizer::Tokenizer()
    {
        this->tokens = std::vector<Token>();
        this->line = 1;
    }

    std::string Tokenizer::TokenizeString(
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

    bool Tokenizer::Tokenize(std::string source)
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

            Token token;
            token.type = TokenType::Unrecognised;
            token.value = "";
            token.line = this->line;

            switch(source[i])
            {
                case ';':
                    token.type = TokenType::Semicolon;
                    token.value = ";";
                break;
                case ',':
                    token.type = TokenType::Comma;
                    token.value = ",";
                break;
                case '=':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::Equal;
                        token.value = "==";
                    }
                    else
                    {
                        token.type = TokenType::Assign;
                        token.value = "=";
                    }
                break;
                case '&':
                    token.type = TokenType::And;
                    token.value = "&";
                break;
                case '|':
                    token.type = TokenType::Or;
                    token.value = "|";
                break;
                case '^':
                    token.type = TokenType::Xor;
                    token.value = "^";
                break;
                case '!':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::NotEqual;
                        token.value = "!=";
                    }
                    else
                    {
                        token.type = TokenType::Not;
                        token.value = "!";
                    }
                break;
                case '+':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::AddAssign;
                        token.value = "+=";
                    }
                    else
                    {
                        token.type = TokenType::Add;
                        token.value = "+";
                    }
                break;
                case '-':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::SubtractAssign;
                        token.value = "-=";
                    }
                    else
                    {
                        token.type = TokenType::Subtract;
                        token.value = "-";
                    }
                break;
                case '*':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::MultiplyAssign;
                        token.value = "*=";
                    }
                    else
                    {
                        token.type = TokenType::Multiply;
                        token.value = "*";
                    }
                break;
                case '/':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::DivideAssign;
                        token.value = "/=";
                    }
                    else
                    {
                        token.type = TokenType::Divide;
                        token.value = "/";
                    }
                break;
                case '<':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::SmallerOrEqual;
                        token.value = "<=";
                    }
                    else
                    {
                        token.type = TokenType::Smaller;
                        token.value = "<";
                    }
                break;
                case '>':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::LargerOrEqual;
                        token.value = ">=";
                    }
                    else
                    {
                        token.type = TokenType::Larger;
                        token.value = ">";
                    }
                break;
                case '(':
                    token.type = TokenType::OpenParam;
                    token.value = "(";
                break;
                case ')':
                    token.type = TokenType::CloseParam;
                    token.value = ")";
                break;
                case '[':
                    token.type = TokenType::OpenBracket;
                    token.value = "[";
                break;
                case ']':
                    token.type = TokenType::CloseBracket;
                    token.value = "]";
                break;
                case '{':
                    token.type = TokenType::OpenScope;
                    token.value = "{";
                break;
                case '}':
                    token.type = TokenType::CloseScope;
                    token.value = "}";
                break;
                case '\"':
                    token.type = TokenType::String;
                    token.value = this->TokenizeString(source, i);
                break;
                case ':':
                    if((source[++i] = ':'))
                    {
                        token.type = TokenType::ScopeResolution;
                        token.value = "::";
                    }
                    else
                    {
                        i--;
                        token.type = TokenType::Unrecognised;
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
                            token.type = TokenType::Function;
                        }
                        else if(token.value == "if")
                        {
                            token.type = TokenType::If;
                        }
                        else if(token.value == "ifn")
                        {
                            token.type = TokenType::Ifn;
                        }
                        else if(token.value == "else")
                        {
                            token.type = TokenType::Else;
                        }
                        else if(token.value == "while")
                        {
                            token.type = TokenType::While;
                        }
                        else if(token.value == "struct")
                        {
                            token.type = TokenType::Struct;
                        }
                        else if(token.value == "namespace")
                        {
                            token.type = TokenType::Namespace;
                        }
                        else if(token.value == "class")
                        {
                            token.type = TokenType::Class;
                        }
                        else if(token.value == "method")
                        {
                            token.type = TokenType::Method;
                        }
                        else if(token.value == "new")
                        {
                            token.type = TokenType::New;
                        }
                        else if(token.value == "delete")
                        {
                            token.type = TokenType::Delete;
                        }
                        else if(token.value == "ref")
                        {
                            token.type = TokenType::Reference;
                        }
                        else if(token.value == "return")
                        {
                            token.type = TokenType::Return;
                        }
                        else if(token.value == "int8")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "uint8")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "int16")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "uint16")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "int32" || token.value == "int")
                        {
                            token.value = "int32";
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "uint32")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "int64" || token.value == "long")
                        {
                            token.value = "int64";
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "uint64")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "float32")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "float64")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else if(token.value == "string")
                        {
                            token.type = TokenType::Primitive;
                        }
                        else
                        {
                            token.type = TokenType::Identifier;
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
                        token.type = TokenType::Number;
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

    std::vector<Token> Tokenizer::GetTokens(){return this->tokens;}
}