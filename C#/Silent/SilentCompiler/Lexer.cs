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
            temp = source.Replace('\n', ' ').Split(' ').ToList();


            for(int i = 0; i < temp.Count; i++)
            {
                if (temp[i].Contains("+") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("+"))
                    {
                        temp1.Add("+");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("+"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("+"))
                    {
                        temp[i].Replace("+", " + ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("+");
                    }
                    continue;

                }

                else if (temp[i].Contains("-") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("-"))
                    {
                        temp1.Add("-");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("-"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("-"))
                    {
                        temp[i].Replace("-", " - ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("-");
                    }
                    continue;
                }

                else if (temp[i].Contains("/") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("/"))
                    {
                        temp1.Add("/");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("/"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("/"))
                    {
                        temp[i].Replace("/", " / ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("/");
                    }
                    continue;
                }

                else if (temp[i].Contains("*") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("*"))
                    {
                        temp1.Add("*");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("*"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("*"))
                    {
                        temp[i].Replace("*", " * ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("*");
                    }
                    continue;
                }

                else if (temp[i].Contains("==") && temp[i].Length > 2)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("=="))
                    {
                        temp1.Add("==");
                        temp[i].Remove(0,2);
                    }

                    if (temp[i].EndsWith("=="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2,2);
                    }

                    if (temp[i].Contains("=="))
                    {
                        temp[i].Replace("==", " == ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("==");
                    }
                    continue;
                }

                else if (temp[i].Contains("!=") && temp[i].Length > 2)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("!="))
                    {
                        temp1.Add("!=");
                        temp[i].Remove(0,2);
                    }

                    if (temp[i].EndsWith("!="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2,2);
                    }

                    if (temp[i].Contains("!="))
                    {
                        temp[i].Replace("!=", " != ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("!=");
                    }
                    continue;
                }

                else if (temp[i].Contains(">=") && temp[i].Length > 2)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith(">="))
                    {
                        temp1.Add(">=");
                        temp[i].Remove(0,2);
                    }

                    if (temp[i].EndsWith(">="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2,2);
                    }

                    if (temp[i].Contains(">="))
                    {
                        temp[i].Replace(">=", " >= ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add(">=");
                    }
                    continue;
                }

                else if (temp[i].Contains("<=") && temp[i].Length > 2)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("<="))
                    {
                        temp1.Add("<=");
                        temp[i].Remove(0,2);
                    }

                    if (temp[i].EndsWith("<="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2,2);
                    }

                    if (temp[i].Contains("<="))
                    {
                        temp[i].Replace("<=", " <= ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("<=");
                    }
                    continue;
                }

                else if (temp[i].Contains(";") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith(";"))
                    {
                        temp1.Add(";");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith(";"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains(";"))
                    {
                        temp[i].Replace(";", " ; ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add(";");
                    }
                    continue;
                }

                else if (temp[i].Contains("(") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("("))
                    {
                        temp1.Add("(");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("("))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("("))
                    {
                        temp[i].Replace("(", " ( ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("(");
                    }
                    continue;
                }

                else if (temp[i].Contains(")") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith(")"))
                    {
                        temp1.Add(")");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith(")"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains(")"))
                    {
                        temp[i].Replace(")", " ) ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add(")");
                    }
                    continue;
                }

                else if (temp[i].Contains("{") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("{"))
                    {
                        temp1.Add("{");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("{"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("{"))
                    {
                        temp[i].Replace("{", " { ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("{");
                    }
                    continue;
                }

                else if (temp[i].Contains("}") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("}"))
                    {
                        temp1.Add("}");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("}"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("}"))
                    {
                        temp[i].Replace("}", " } ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("}");
                    }
                    continue;
                }

                else if (temp[i].Contains("[") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("["))
                    {
                        temp1.Add("[");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("["))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("["))
                    {
                        temp[i].Replace("[", " [ ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("[");
                    }
                    continue;
                }

                else if (temp[i].Contains("]") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("]"))
                    {
                        temp1.Add("]");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("]"))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("]"))
                    {
                        temp[i].Replace("]", " ] ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("]");
                    }
                    continue;
                }

                else if (temp[i].Contains("=") && temp[i].Length > 1)
                {
                    bool AddToEnd = false;

                    if (temp[i].StartsWith("="))
                    {
                        temp1.Add("=");
                        temp[i].Remove(0);
                    }

                    if (temp[i].EndsWith("="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-1);
                    }

                    if (temp[i].Contains("="))
                    {
                        temp[i].Replace("=", " = ");
                        string[] temp2 = temp[i].Split(' ');
                        for (int x = 0; x < temp2.Length; x++)
                        {
                            temp1.Add(temp2[x]);
                        }
                    }

                    if (AddToEnd)
                    {
                        temp1.Add("=");
                    }
                    continue;
                }

                else
                {
                    temp1.Add(temp[i]);
                }

            }

            for(int i = 0; i < temp1.Count; i++)
            {
                if (temp1[i] == "new") { tokens.Add(Tokens.New); }
                if (temp1[i] == "void") { tokens.Add(Tokens.Void); }
                if (temp1[i] == "int") { tokens.Add(Tokens.Integer); }
                if (temp1[i] == "float") { tokens.Add(Tokens.Float); }
                if (temp1[i] == "char") { tokens.Add(Tokens.Char); }
                if (temp1[i] == "+") { tokens.Add(Tokens.Add); }
                if (temp1[i] == "-") { tokens.Add(Tokens.Subtract); }
                if (temp1[i] == "*") { tokens.Add(Tokens.Multiple); }
                if (temp1[i] == "/") { tokens.Add(Tokens.Divide); }
                if (temp1[i] == "==") { tokens.Add(Tokens.Equal); }
                if (temp1[i] == "=") { tokens.Add(Tokens.Assign); }
                if (temp1[i] == ">=") { tokens.Add(Tokens.MoreThan); }
                if (temp1[i] == "<=") { tokens.Add(Tokens.LessThan); }
                if (temp1[i] == "!=") { tokens.Add(Tokens.IsNot); }
                if (temp1[i] == ";") { tokens.Add(Tokens.Semicolon); }
                if (temp1[i] == "[") { tokens.Add(Tokens.OpenBracket); }
                if (temp1[i] == "]") { tokens.Add(Tokens.CloseBracket); }
                if (temp1[i] == "(") { tokens.Add(Tokens.OpenParentheses); }
                if (temp1[i] == ")") { tokens.Add(Tokens.CloseParentheses); }
                if (temp1[i] == "{") { tokens.Add(Tokens.OpenCurlyBracket); }
                if (temp1[i] == "}") { tokens.Add(Tokens.CloseCurlyBracket); }
                if (temp1[i] == "return") { tokens.Add(Tokens.Return); }
                if (temp1[i] == "using") { tokens.Add(Tokens.Using); }
                if (temp1[i] == "public") { tokens.Add(Tokens.Public); }
                if (temp1[i] == "private") { tokens.Add(Tokens.Private); }
                if (temp1[i] == "static") { tokens.Add(Tokens.Static); }
                if (temp1[i] == "class") { tokens.Add(Tokens.Class); }
                if (temp1[i] == "while") { tokens.Add(Tokens.Class); }
                if (temp1[i] == "for") { tokens.Add(Tokens.Class); }
                if (temp1[i] == "if") { tokens.Add(Tokens.Class); }
                if (temp1[i] == "elif") { tokens.Add(Tokens.Class); }
                if (temp1[i] == "else") { tokens.Add(Tokens.Class); }
            }

            return tokens;
        }
    }
}
