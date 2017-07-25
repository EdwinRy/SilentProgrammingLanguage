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
        List<string> values;
        List<Tokens> tokens;


        public void Compile(string source)
        {
            Lexer lexer = new Lexer();
            tokens = lexer.Tokenize(source, out values);
            Parser parser = new Parser(tokens, values);

            for(int i = 0; i < tokens.Count; i++)
            {
                Console.WriteLine(tokens[i]);
            }

            Console.WriteLine("values:");

            for (int i = 0; i < values.Count; i++)
            {
                Console.WriteLine(values[i]);
            }
        }
    }

    //All possible tokens in the language
    public enum Tokens
    {
        Public,              
        Private,
        Protected,
        Static,              

        Namespace,           
        Using,
        Class,               
        Fullstop,

        Method,
        Function,            

        Return,
        Struct,              
        Array,               

        While,
        For,

        If,
        Elif,
        Else,

        New,                 
        Assign,
        Void,               
        Integer,     
        Float,
        String,
        Char,
        Variable,
        Comma,

        Add,              
        Subtract,         
        Multiple,         
        Divide,
        
        Equal,            
        IsNot,
        MoreThan,
        LessThan,

        Semicolon,
        OpenBracket,
        CloseBracket,
        OpenParentheses,
        CloseParentheses,
        OpenCurlyBracket,
        CloseCurlyBracket,

        Value              
    }
}

