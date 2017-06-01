using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace SilentCompiler
{
    public class SilentCompiler
    {
        List<Tokens> tokens;


        public void Compile(string source)
        {
            Lexer lexer = new Lexer();
            tokens = lexer.Tokenize(source);
        }
    }

    public enum Tokens
    {
        New,      
        Void,       
        Integer,     
        Float,       
        Char,
        Add,              
        Subtract,         
        Multiple,         
        Divide,           
        Equal,            
        Assign,
        MoreThan,
        LessThan,
        IsNot,
        Semicolon,
        OpenBracket,
        CloseBracket,
        OpenParentheses,
        CloseParentheses,
        OpenCurlyBracket,
        CloseCurlyBracket,
        Return,
        Using
    }

}

