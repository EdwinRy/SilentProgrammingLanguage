#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    typedef unsigned long long uint64;
    enum class TokenType
    {
        Unrecognised,
        Identifier,
        Number,
        String,
        OpenParam,
        CloseParam,
        OpenBracket,
        CloseBracket,
        OpenScope,
        CloseScope,
        Multiply,
        MultiplyAssign,
        Divide,
        DivideAssign,
        Add,
        AddAssign,
        Subtract,
        SubtractAssign,
        Semicolon,
        Primitive,
        Comma,
        FullStop,
        If,
        Ifn,
        Else,
        While,
        Assign,
        Equal,
        NotEqual,
        Larger,
        LargerOrEqual,
        Smaller,
        SmallerOrEqual,
        And,
        Or,
        Xor,
        Not,
        New,
        Delete,
        //Reference,
        Function,
        Struct,
        Return,
        Namespace,
        Class,
        Method,
        ScopeResolution
    };

    typedef struct Token
    {
        TokenType type;
        std::string value;
        unsigned long long line;
    }Token;


    class Tokenizer
    {
        private:
        std::string TokenizeString(std::string source, uint64 &index);
        std::vector<Token> tokens;
        uint64 line;

        public:
        Tokenizer();
        bool Tokenize(std::string source);
        std::vector<Token> GetTokens();

    };
}