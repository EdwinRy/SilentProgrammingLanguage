﻿using System;
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
            temp = source.Replace('\n', ' ').Split(' ').ToList();


            for(int i = 0; i < temp.Count; i++)
            {
                if (temp[i] == null) temp.RemoveAt(i);
            }

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

                    Console.WriteLine(temp[i]);

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
                        Console.WriteLine("true");
                        temp[i].Replace("(", " ( ");
                        Console.WriteLine(temp[i].Replace("("," ( "));
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
