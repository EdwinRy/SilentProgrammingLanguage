#include "STokenizer.hpp"
namespace Silent
{
    Tokenizer::Tokenizer()
    {
        this->tokens = std::vector<Token>();
        this->line = 1;
    }

    std::string Tokenizer::TokenizeString(
        std::string source, unsigned long long &index
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
        for(unsigned long long i = 0; i < source.length(); i++)
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
            token.type = TokenType::Identifier;
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
                case '.':
                    token.type = TokenType::Fullstop;
                    token.value = ".";
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
                    if (source[i + 1] == '&&')
                    {
                        i++;
                        token.type = TokenType::ConditionalAnd;
                        token.value = "&&";
                    }
                    else
                    {
                        token.type = TokenType::And;
                        token.value = "&";
                    }
                break;
                case '|':
                    if (source[i + 1] == '|')
                    {
                        i++;
                        token.type = TokenType::ConditionalOr;
                        token.value = "||";
                    }
                    else
                    {
                        token.type = TokenType::Or;
                        token.value = "|";
                    }
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
                        token.type = TokenType::LessThanOrEqualTo;
                        token.value = "<=";
                    }
                    else
                    {
                        token.type = TokenType::LessThan;
                        token.value = "<";
                    }
                break;
                case '>':
                    if(source[i+1] == '=')
                    {
                        i++;
                        token.type = TokenType::MoreThanOrEqualTo;
                        token.value = ">=";
                    }
                    else
                    {
                        token.type = TokenType::MoreThan;
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
                    token.type = TokenType::StringValue;
                    token.value = this->TokenizeString(source, i);
                break;
                case ':':
                    if((source[++i] == ':'))
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


                        //  Keywords

                        if(token.value == "sub")
                        {
                            token.type = TokenType::Subroutine;
                        }
                        else if(token.value == "if")
                        {
                            token.type = TokenType::If;
                        }
                        else if(token.value == "ifn")
                        {
                            token.type = TokenType::IfNot;
                        }
                        else if(token.value == "else")
                        {
                            token.type = TokenType::Else;
                        }
                        else if(token.value == "for")
                        {
                            token.type = TokenType::While;
                        }
                        else if(token.value == "while")
                        {
                            token.type = TokenType::While;
                        }
                        else if(token.value == "type")
                        {
                            token.type = TokenType::Type;
                        }
                        else if(token.value == "return")
                        {
                            token.type = TokenType::Return;
                        }
                        else if(token.value == "new")
                        {
                            token.type = TokenType::New;
                        }
                        else if(token.value == "delete")
                        {
                            token.type = TokenType::Delete;
                        }
                        else if(token.value == "namespace")
                        {
                            token.type = TokenType::Namespace;
                        }
                        else if(token.value == "import")
                        {
                            token.type = TokenType::Import;
                        }
                        else if(token.value == "inline")
                        {
                            token.type = TokenType::Inline;
                        }
                        else if(token.value == "asm")
                        {
                            token.type = TokenType::Asm;
                        }
                        else if (token.value == "public")
                        {
                            token.type = TokenType::Public;
                        }
                        else if (token.value == "private")
                        {
                            token.type = TokenType::Private;
                        }
                        else if (token.value == "protected")
                        {
                            token.type = TokenType::Protected;
                        }

                        // Primitives
                        else if (token.value == "void")
                        {
                            token.type = TokenType::Void;
                        }

                        else if(token.value == "int8")
                        {
                            token.type = TokenType::I8;
                        }
                        else if(token.value == "uint8")
                        {
                            token.type = TokenType::UI8;
                        }
                        else if(token.value == "int16")
                        {
                            token.type = TokenType::I16;
                        }
                        else if(token.value == "uint16")
                        {
                            token.type = TokenType::UI16;
                        }
                        else if(token.value == "int32" || token.value == "int")
                        {
                            token.value = "int32";
                            token.type = TokenType::I32;
                        }
                        else if(token.value == "uint32" ||
                            token.value == "uint")
                        {
                            token.value = "uint32";
                            token.type = TokenType::UI32;
                        }
                        else if(token.value == "int64" || 
                            token.value == "long")
                        {
                            token.value = "int64";
                            token.type = TokenType::I64;
                        }
                        else if(token.value == "uint64" || 
                            token.value == "ulong")
                        {
                            token.value = "uint64";
                            token.type = TokenType::UI64;
                        }
                        else if(token.value == "float32" ||
                            token.value == "float")
                        {
                            token.value = "float32";
                            token.type = TokenType::F32;
                        }
                        else if(token.value == "float64")
                        {
                            token.type = TokenType::F64;
                        }
                        else if(token.value == "string")
                        {
                            token.type = TokenType::String;
                        }
                        else
                        {
                            token.type = TokenType::Identifier;
                        }
                    }
                    else if(isdigit(source[i]))
                    {
                        while(isdigit(source[i]) /*|| source[i] == '.'*/)
                        {
                            token.value += source[i];
                            i++;
                        }
                        i--;
                        token.type = TokenType::Digits;
                    }
                    else
                    {
                        continue;
                    }
                break;
            }
            this->tokens.push_back(token);
        }
        return true;
    }

    bool Token::IsPrimitive()
    {
        switch(type)
        {
            case TokenType::Void:
            case TokenType::I8:
            case TokenType::UI8:
            case TokenType::I16:
            case TokenType::UI16:
            case TokenType::I32:
            case TokenType::UI32:
            case TokenType::I64:
            case TokenType::UI64:
            case TokenType::F32:
            case TokenType::F64:
            case TokenType::String:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsKeyword()
    {
        switch(type)
        {
            case TokenType::Subroutine:
            case TokenType::If:
            case TokenType::IfNot:
            case TokenType::Else:
            case TokenType::For:
            case TokenType::While:
            case TokenType::Type:
            case TokenType::Return:
            case TokenType::New:
            case TokenType::Delete:
            case TokenType::Namespace:
            case TokenType::Import:
            // case TokenType::Export:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsAccessModifier()
    {
        switch (type)
        {
            case TokenType::Public:
            case TokenType::Private:
            case TokenType::Protected:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsArithmetic()
    {
        switch(type)
        {
            case TokenType::Add:
            case TokenType::Subtract:
            case TokenType::Multiply:
            case TokenType::Divide:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsSign()
    {
        switch(type)
        {
            case TokenType::Add:
            case TokenType::Subtract:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsComparison()
    {
        switch(type)
        {
            case TokenType::Equal:
            case TokenType::NotEqual:
            case TokenType::LessThan:
            case TokenType::LessThanOrEqualTo:
            case TokenType::MoreThan:
            case TokenType::MoreThanOrEqualTo:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsLogic()
    {
        switch(type)
        {
            case TokenType::And:
            case TokenType::Or:
            case TokenType::Xor:
            case TokenType::Not:
                return true;
            break;

            default:
                return false;
            break;
        }
    }
    
    bool Token::IsBitShift()
    {
        switch(type)
        {
            case TokenType::RightShift:
            case TokenType::LeftShift:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsAssignment()
    {
        switch(type)
        {
            case TokenType::Assign:
            case TokenType::AddAssign:
            case TokenType::SubtractAssign:
            case TokenType::MultiplyAssign:
            case TokenType::DivideAssign:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsOtherSymbol()
    {
        switch(type)
        {
            case TokenType::Semicolon:
            case TokenType::Comma:
            case TokenType::Fullstop:
            case TokenType::OpenParam:
            case TokenType::CloseParam:
            case TokenType::OpenBracket:
            case TokenType::CloseBracket:
            case TokenType::OpenScope:
            case TokenType::CloseScope:
            // case TokenType::ScopeResolution:
                return true;
            break;

            default:
                return false;
            break;
        }
    }

    bool Token::IsOther()
    {
        switch(type)
        {
            case TokenType::Identifier:
            case TokenType::Digits:
            case TokenType::StringValue:
            case TokenType::Unrecognised:
                return true;
            break;
            
            default:
                return false;
            break;
        }
    }

    std::vector<Token> Tokenizer::GetTokens()
    {
        return this->tokens;
    }
}