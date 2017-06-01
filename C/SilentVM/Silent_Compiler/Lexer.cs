using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentCompiler
{
    public class Lexer
    {
        List<Tokens> tokens;

        List<string> temp;
        List<string> temp1;

        public List<Tokens> Tokenize(string source)
        {
            tokens = new List<Tokens>();
            temp1 = new List<string>();
            temp = source.Replace('\n', ' ').
                Replace('\t', ' ').
                Replace('\r', ' ').
                Replace("+"," + ").
                Replace("-", " - ").
                Replace("/", " / ").
                Replace("*", " * ").
                Replace("==", " == ").
                Replace("!=", " != ").
                Replace(">=", " >= ").
                Replace("<=", " <= ").
                Replace(";", " ; ").
                Replace("(", " ( ").
                Replace(")", " ) ").
                Replace("{", " { ").
                Replace("}", " } ").
                Replace("[", " [ ").
                Replace("]", " ] ").
                Replace("=", " = ").
                Split(' ').ToList();


            for (int i = 0; i < temp.Count; i++)
            {
                if(temp[i] != string.Empty)
                temp1.Add(temp[i]);
            }

            for(int i = 0; i < temp1.Count; i++)
            {
                Console.WriteLine(temp1[i]);
                if (temp1[i] == "new") { tokens.Add(Tokens.New); }
                else if (temp1[i] == "void") { tokens.Add(Tokens.Void); }
                else if (temp1[i] == "int") { tokens.Add(Tokens.Integer); }
                else if (temp1[i] == "float") { tokens.Add(Tokens.Float); }
                else if (temp1[i] == "char") { tokens.Add(Tokens.Char); }
                else if (temp1[i] == "+") { tokens.Add(Tokens.Add); }
                else if (temp1[i] == "-") { tokens.Add(Tokens.Subtract); }
                else if (temp1[i] == "*") { tokens.Add(Tokens.Multiple); }
                else if (temp1[i] == "/") { tokens.Add(Tokens.Divide); }
                else if (temp1[i] == "==") { tokens.Add(Tokens.Equal); }
                else if (temp1[i] == "=") { tokens.Add(Tokens.Assign); }
                else if (temp1[i] == ">=") { tokens.Add(Tokens.MoreThan); }
                else if (temp1[i] == "<=") { tokens.Add(Tokens.LessThan); }
                else if (temp1[i] == "!=") { tokens.Add(Tokens.IsNot); }
                else if (temp1[i] == ";") { tokens.Add(Tokens.Semicolon); }
                else if (temp1[i] == "[") { tokens.Add(Tokens.OpenBracket); }
                else if (temp1[i] == "]") { tokens.Add(Tokens.CloseBracket); }
                else if (temp1[i] == "(") { tokens.Add(Tokens.OpenParentheses); }
                else if (temp1[i] == ")") { tokens.Add(Tokens.CloseParentheses); }
                else if (temp1[i] == "{") { tokens.Add(Tokens.OpenCurlyBracket); }
                else if (temp1[i] == "}") { tokens.Add(Tokens.CloseCurlyBracket); }
                else if (temp1[i] == "return") { tokens.Add(Tokens.Return); }
                else if (temp1[i] == "using") { tokens.Add(Tokens.Using); }
                else if (temp1[i] == "public") { tokens.Add(Tokens.Public); }
                else if (temp1[i] == "private") { tokens.Add(Tokens.Private); }
                else if (temp1[i] == "static") { tokens.Add(Tokens.Static); }
                else if (temp1[i] == "class") { tokens.Add(Tokens.Class); }
                else if (temp1[i] == "while") { tokens.Add(Tokens.Class); }
                else if (temp1[i] == "for") { tokens.Add(Tokens.Class); }
                else if (temp1[i] == "if") { tokens.Add(Tokens.Class); }
                else if (temp1[i] == "elif") { tokens.Add(Tokens.Class); }
                else if (temp1[i] == "else") { tokens.Add(Tokens.Class); }
                else if (temp1[i] == "struct") { tokens.Add(Tokens.Struct); }
                else { tokens.Add(Tokens.Value); }
            }

            return tokens;
        }
    }
}
