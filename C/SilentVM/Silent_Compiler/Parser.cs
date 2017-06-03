using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentCompiler
{
    class Parser
    {
        List<Tokens> tokens;
        List<string> values;

        List<silent_Namespace> namespaces;
        List<silent_Class> classes;
        List<silent_Struct> structs;
        List<silent_Variable> variables;
        List<silent_Array> arrays;
        List<silent_Function> functions;
        List<silent_Expression> expressions;

        public void Parse(List<Tokens> tokens, List<string> values)
        {
            this.tokens = tokens;
            this.values = values;

            namespaces = new List<silent_Namespace>();
            classes = new List<silent_Class>();
            structs = new List<silent_Struct>();
            variables = new List<silent_Variable>();
            arrays = new List<silent_Array>();
            functions = new List<silent_Function>();
            expressions = new List<silent_Expression>();

            for (int i = 0; i <= tokens.Count; i++)
            {
                if(tokens[i] == Tokens.Namespace)
                {
                    namespaces.Add(sortNamespace(i));
                }

                if(tokens[i] == Tokens.Function)
                {
                    functions.Add(sortFunction(i));
                }
            }
        }

        List<silent_Expression> sortExpression(int pos)
        {
            List<silent_Expression> expression = new List<silent_Expression>();

            return expression;
        }

        silent_Function sortFunction(int pos)
        {
            silent_Function function = new silent_Function();
            function.returnType = (Types)tokens[++pos];

            return function;
        }

        silent_Namespace sortNamespace(int pos)
        {
            //not completed
            int startPos = pos;
            int endPos;

            int noScopes = 0;

            for(int i = startPos + 3; i < tokens.Count; i++)
            {
                if(i >= startPos)
                {
                    if(tokens[i] == Tokens.OpenBracket)
                    {
                        noScopes++;
                    }

                    if(tokens[i] == Tokens.CloseBracket)
                    {
                        noScopes--;
                    }

                    if(noScopes < 0)
                    {
                        endPos = i;
                        tokens.RemoveRange(startPos, (endPos - startPos));
                        break;
                    }

                }

            }

            silent_Namespace Namespace = new silent_Namespace();
            Namespace.name = this.values[CountVal(pos)];
            
            
            return Namespace;
        }

        int CountVal(int pos)
        {
            int counter = 0;

            for(int i = 0; i < pos; i++)
            {
                if (tokens[i] == Tokens.Value) counter++;
            }

            return counter;
        }

    }

    struct silent_Namespace
    {
        public string name;
        public List<silent_Function> functions;
        public List<silent_Variable> variables;
        public List<silent_Class> classes;
    }

    struct silent_Class
    {
        public string name;
        public List<silent_Method> methods;
        public List<silent_ClassVariable> members;
        public silent_Constructor constructor;
    }

    struct silent_Variable
    {
        public string name;
        public Types variableType;
        public string data;
    }

    struct silent_ClassVariable
    {
        public string name;
        public Types variableType;
        public string data;
        public silent_Class owner;
        public AccessModifiers accessModifier;
    }   

    struct silent_Expression
    {
        public string name;
        public List<Tokens> tokens;
        public List<string> values;
    }

    struct silent_Array
    {
        public string name;
        public List<silent_Variable> variables;
        public int Length;
    }

    struct silent_Struct
    {
        public string name;
        public List<silent_Variable> variables;
        public List<silent_Array> arrays;
    }

    struct silent_Constructor
    {
        public string name;
        public List<silent_Expression> expressions;

        public List<silent_Variable> localVariables;
        public List<silent_Struct> localStructs;
        public List<silent_Array> localArrays;
    }

    struct silent_Function
    {
        public string name;
        public List<silent_Expression> expressions;
        public Types returnType;
        public silent_Variable returnValue;

        public List<silent_Variable> localVariables;
        public List<silent_Struct> localStructs;
        public List<silent_Array> localArrays;
    }

    struct silent_Method
    {
        public AccessModifiers access;
        public string name;
        public List<silent_Expression> expressions;
        public silent_Variable returnType;

        public List<silent_Variable> localVariables;
        public List<silent_Struct> localStructs;
        public List<silent_Array> localArrays;
    }

    enum Types
    {
        Char = Tokens.Char,
        String = Tokens.String,
        Integer = Tokens.Integer,
        Float = Tokens.Float,
        Null = Tokens.Void
    }

    enum AccessModifiers
    {
        Private,
        Public
    }

    enum ByteCode
    {
       Halt,
       GoTo,
       Call,

       ClearMemory,
       ClearStorage,

       PushByte,
       PushInt,
       PushFloat,

       PopByte,
       PopInt,
       PopFloat,

       StoreByte,
       StoreInt,
       StoreFloat,

       LoadByte,
       LoadInt,
       LoadFloat,

       SetByte,
       SetInt,
       SetFloat,

       AddByte,
       AddInt,
       AddFloat,

       SubtractByte,
       SubtractInt,
       SubtractFloat,

       MultiplyByte,
       MultiplyInt,
       MultiplyFloat,

       DivideByte,
       DivideInt,
       DivideFloat,

       Byte2Int,
       Byte2Float,
       Int2Float,
       Float2Int,

       SmallerThan,
       BiggerThan,
       Equal,

       If,
       IfNot,

       And,
       Or,
       Not,
    }
}
