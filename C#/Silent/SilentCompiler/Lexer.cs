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

            for(int i = 0;i < temp.Count; i++)
            {

                if (temp[i].Contains("new")){}
                else if (temp[i].Contains("void")) { }
                else if (temp[i].Contains("int")) { }
                else if (temp[i].Contains("float")) { }
                else if (temp[i].Contains("char")) { }

            }



            return tokens;
        }
    }
}
