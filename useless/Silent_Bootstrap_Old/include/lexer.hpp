#pragma once
namespace Silent
{
    enum class TokenType
    {
        // Primitives
        Void,
        I8,
        UI8,
        I16,
        UI16,
        I32,
        UI32,
        I64,
        UI64,
        F32,
        F64,
        String,

        // Keywords
        Subroutine,
        If,
        IfNot,
        Else,
        For,
        While,
        Type,
        Return,
        New,
        Delete,
        Namespace,
        Import,
        Inline,

        // Access modifiers
        Public,
        Private,
        Protected,

        // Export,
        Asm,

        // Arithmetic Symbols
        Add,
        Subtract,
        Multiply,
        Divide,

        // Comparison Operators
        Equal,
        NotEqual,
        LessThan,
        LessThanOrEqualTo,
        MoreThan,
        MoreThanOrEqualTo,

        // Logical Operators
        And,
        Or,
        Xor,
        Not,

        // Conditional Operator
        ConditionalAnd,
        ConditionalOr,

        // Bit Shift Operators
        RightShift,
        LeftShift,

        // Assignment Operators
        Assign,
        AddAssign,
        SubtractAssign,
        MultiplyAssign,
        DivideAssign,

        // Other Symbols
        Semicolon,
        Comma,
        Fullstop,
        OpenParam,
        CloseParam,
        OpenBracket,
        CloseBracket,
        OpenScope,
        CloseScope,
        Colon,

        // Other
        Identifier,
        Digits,
        StringValue,
        Unrecognised,
        EndOfFile
    };

    struct Token
    {
        char* Val;
        TokenType Type;
        unsigned long long Line;
    };

    class Lexer
    {
        public:
        Lexer(char* source);
        static Token* Tokenize(char* source);

        private:

        static TokenType GetTokenType(char* str);
        static bool IsIdStart(char c);
        static bool IsNumStart(char c);
        static bool SkipComment(char* source, uint64_t& i, uint64_t& line);
        static char* ParseStringValue(char* source, uint64_t& i, uint64_t& line);
    };
}