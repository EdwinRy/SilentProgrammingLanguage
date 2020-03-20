#include <iostream>
#include <vector>

#include "lexer.hpp"

namespace Silent
{
    using std::vector;
    using std::string;

    Lexer::Lexer(char* source)
    {

    }

    Token* Lexer::Tokenize(char* source)
    {
        vector<Token> tokens = vector<Token>();
        uint64_t line = 1;

        for (uint64_t i = 0; source[i] != 0; i++)
        {
            while(SkipComment(source, i, line));

            Token token;
            switch (source[i])
            {
                case '\n':
                    line++;
                    continue;
                    break;
                case ';':
                    token.Type = TokenType::Semicolon;
                    break;

                case ',':
                    token.Type = TokenType::Comma;
                    break;

                case '.':
                    token.Type = TokenType::Fullstop;
                    break;

                case '=':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::Equal;
                    }
                    else
                    {
                        token.Type = TokenType::Assign;
                    }
                    break;

                case '&':
                    if (source[i + 1] == '&&')
                    {
                        i++;
                        token.Type = TokenType::ConditionalAnd;
                    }
                    else
                    {
                        token.Type = TokenType::And;
                    }
                    break;

                case '|':
                    if (source[i + 1] == '|')
                    {
                        i++;
                        token.Type = TokenType::ConditionalOr;
                    }
                    else
                    {
                        token.Type = TokenType::Or;
                    }
                    break;

                case '^':
                    token.Type = TokenType::Xor;
                    break;

                case '!':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::NotEqual;
                    }
                    else
                    {
                        token.Type = TokenType::Not;
                    }
                    break;

                case '+':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::AddAssign;
                    }
                    else
                    {
                        token.Type = TokenType::Add;
                    }
                    break;

                case '-':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::SubtractAssign;
                    }
                    else
                    {
                        token.Type = TokenType::Subtract;
                    }
                    break;

                case '*':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::MultiplyAssign;
                    }
                    else
                    {
                        token.Type = TokenType::Multiply;
                    }
                    break;

                case '/':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::DivideAssign;
                    }
                    else
                    {
                        token.Type = TokenType::Divide;
                    }
                    break;

                case '<':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::LessThanOrEqualTo;
                    }
                    else
                    {
                        token.Type = TokenType::LessThan;
                    }
                    break;

                case '>':
                    if (source[i + 1] == '=')
                    {
                        i++;
                        token.Type = TokenType::MoreThanOrEqualTo;
                    }
                    else
                    {
                        token.Type = TokenType::MoreThan;
                    }
                    break;

                case '(':
                    token.Type = TokenType::OpenParam;
                    break;

                case ')':
                    token.Type = TokenType::CloseParam;
                    break;

                case '[':
                    token.Type = TokenType::OpenBracket;
                    break;

                case ']':
                    token.Type = TokenType::CloseBracket;
                    break;

                case '{':
                    token.Type = TokenType::OpenScope;
                    break;

                case '}':
                    token.Type = TokenType::CloseScope;
                    break;

                case ':':
                    token.Type = TokenType::Colon;
                    break;

                case '\"':
                    token.Type = TokenType::StringValue;
                    token.Val = ParseStringValue(source, i, line);
                    break;

                default:
                    if (IsIdStart(source[i]))
                    {
                        string id;
                        while (IsIdStart(source[i]) || isdigit(source[i]))
                            id += source[i++];

                        token.Val = new char[id.size()];
                        strcpy(token.Val, id.c_str());
                        token.Type = GetTokenType(token.Val);

                    }
                    else if (IsNumStart(source[i]))
                    {
                        string num;
                    }
                    break;
            }
            token.Line = line;
            tokens.push_back(token);
        }

        Token* returnTokens = new Token[tokens.size()];
        memcpy(returnTokens, tokens.data(), tokens.size() * sizeof(Token));
        return returnTokens;
    }

    TokenType Lexer::GetTokenType(char* str)
    {
        string s = string(str);
        switch (str[0])
        {
            case 't':
                if (s == "type") return TokenType::Type;
                break;

            case 'i':
                if (s == "if") return TokenType::If;
                else if (s == "ifn") return TokenType::IfNot;
                else if (s == "int8") return TokenType::I8;
                break;

            case 'e':
                if (s == "else") return TokenType::Else;
                break;

            case 'f':
                if (s == "for") return TokenType::For;
                break;

            case 'w':
                if (s == "while") return TokenType::While;
                break;

            case 'r':
                if (s == "return") return TokenType::Return;
                break;

            case 'n':
                if (s == "namespace") return TokenType::Namespace;
                break;

            //default:
            //    return TokenType::Unrecognised;
            //    break;
        }
        return TokenType::Identifier;
    }

    bool Lexer::IsIdStart(char c)
    {
        if (isalpha(c)) return true;
        if (c == '_') return true;
        return false;
    }

    bool Lexer::IsNumStart(char c)
    {
        if (isdigit(c)) return true;
        else if (c == '.') return true;
        return false;
    }

    bool Lexer::SkipComment(char* source, uint64_t& i, uint64_t& line)
    {
        // Skip single line comment
        if (source[i] == '/')
        {
            switch (source[i + 1])
            {
                // Skip single-line comment
                case '/':
                    i += 2;
                    while (source[i] != '\0')
                    {
                        if (source[i] == '\n')
                        {
                            line++;
                            i++;
                            break;
                        }
                        i++;
                    }
                    return true;
                break;

                // Skip multi-line comment
                case '*':
                    i += 2;
                    while (source[i] != '\0')
                    {
                        if (source[i] == '\n') line++;
                        if (source[i] == '*' && source[i + 1] == '/')
                        {
                            i += 2;
                            break;
                        }
                        i++;
                    }
                    return true;
                break;
            }
        }
        return false;
    }

    char* Lexer::ParseStringValue(char* source, uint64_t& i, uint64_t& line)
    {
        std::string str;
        i++;
        while (source[i] != '\"')
        {
            if (source[i] == '\\')
            {
                i++;
                switch (source[i])
                {
                    case 'n':
                        str += '\n';
                        break;
                    case 't':
                        str += '\t';
                        break;
                    case '\"':
                        str += '\"';
                        break;
                    case '\\':
                        str += '\\';
                        break;
                    case 'v':
                        str += '\v';
                        break;
                    case 'r':
                        str += '\r';
                        break;
                    case 'f':
                        str += '\f';
                        break;
                    case '0':
                        str += '\0';
                        break;
                    default:
                        printf("Incorrect string formatting on line %i\n",
                            (int)line);
                        break;
                }
                i++;
                continue;
            }
            if (source[i] == '\n')
            {
                line++;
            }
            str += source[i++];
        }
        i++;

        char* out = new char[str.length()];
        strcpy(out, str.c_str());
        return out;
    }
}