using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*for(int i = 0; i < temp.Count; i++)
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
                        string[] temp2 = temp[i].Replace("+", " + ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("-", " - ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("/", " / ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("*", " * ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("==", " == ").Split(' ');
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
                        temp[i].Remove(0, 2);
                    }

                    if (temp[i].EndsWith("!="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2, 2);
                    }

                    if (temp[i].Contains("!="))
                    {
                        string[] temp2 = temp[i].Replace("!=", " != ").Split(' ');
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
                        temp[i].Remove(0, 2);
                    }

                    if (temp[i].EndsWith(">="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2, 2);
                    }

                    if (temp[i].Contains(">="))
                    {
                        string[] temp2 = temp[i].Replace(">=", " >= ").Split(' ');
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
                        temp[i].Remove(0, 2);
                    }

                    if (temp[i].EndsWith("<="))
                    {
                        AddToEnd = true;
                        temp[i].Remove(-2, 2);
                    }

                    if (temp[i].Contains("<="))
                    {
                        string[] temp2 = temp[i].Replace("<=", " <= ").Split(' ');
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
                        string[] temp2 = temp[i].Replace(";", " ; ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("(", " ( ").Split(' ');
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
                    i -= 2;
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
                        string[] temp2 = temp[i].Replace(")", " ) ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("{", " { ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("}", " } ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("[", " [ ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("]", " ] ").Split(' ');
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
                        string[] temp2 = temp[i].Replace("=", " = ").Split(' ');
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

            } */

