using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentCompiler
{
    class Parser
    {
        List<string> values;
        List<Tokens> program;

        silent_Scope globalScope;


        public Parser(List<Tokens> tokens, List<string> values)
        {
            this.values = values;
            this.program = tokens;

            globalScope = new silent_Scope();
            globalScope.tokens = tokens;
            globalScope.pos = 0;

            Parse(globalScope);

        }

        public void Parse(silent_Scope scope)
        {

            for (int i = 0; i <= scope.tokens.Count; i++)
            {
                if(scope.tokens[i] == Tokens.Namespace)
                {
                    scope.pos = i;
                    scope.namespaces.Add(PrepareNamespace(scope));
                }

                if(scope.tokens[i] == Tokens.Function)
                {
                    scope.pos = i;
                    scope.functions.Add(PrepareFunction(scope));
                }
            }
        }

        List<silent_Expression> PrepareExpression(silent_Scope scope)
        {
            List<silent_Expression> expression = new List<silent_Expression>();

            return expression;
        }

        silent_Function PrepareFunction(silent_Scope scope)
        {
            silent_Function function = new silent_Function();
            function.returnType = (Types)tokens[++pos];

            return function;
        }

        //Sort namespace in the global scope
        silent_Namespace PrepareNamespace(silent_Scope scope)
        {
            //Create new namespace
            silent_Namespace Namespace = new silent_Namespace();

            //Used to get the length of the scope
            int startPos = scope.pos;
            int endPos;

            //Number of scopes within the namespace
            int noScopes = 0;


            //Isolate the scope from the rest of the program
            for(int i = startPos; i < scope.tokens.Count; i++)
            {
                
                //If opening bracket appears, a new scope is created
                if(scope.tokens[i] == Tokens.OpenBracket)
                {
                    noScopes++;
                }

                //If closing bracket appears, a scope is closed
                if(scope.tokens[i] == Tokens.CloseBracket)
                {
                    noScopes--;
                }

                //If the closing bracket closes current namespace scope
                if(noScopes < 0)
                {
                    endPos = i;

                    //Copy the namespace code into the namespace object
                    for(int x = startPos; x <= endPos; x++)
                    {
                        Namespace.tokens.Add(scope.tokens[x]);
                    }

                    //Remove used code from the global scope as it won't be needed
                    scope.tokens.RemoveRange(startPos, (endPos - startPos));
                    break;
                }
                
            }

            if()

            Namespace.name = this.values[CountVal(pos)];
            
            
            return Namespace;
        }

        int CountVal(int pos)
        {
            int counter = 0;

            for(int i = 0; i < pos; i++)
            {
                if (globalScope.tokens[i] == Tokens.Value) counter++;
            }

            return counter;
        }

    }

    struct silent_Scope
    {
        public List<silent_Namespace> namespaces;
        public List<silent_Class> classes;
        public List<silent_Struct> structs;
        public List<silent_Variable> variables;
        public List<silent_Array> arrays;
        public List<silent_Function> functions;
        public List<silent_Expression> expressions;
        public int pos;
        public List<Tokens> tokens;
        public List<silent_Scope> scopes;

    }

    struct silent_Namespace
    {
        public string name;
        public int pos;
        public List<silent_Function> functions;
        public List<silent_Variable> variables;
        public List<silent_Class> classes;
        public List<Tokens> tokens;
    }

    struct silent_Class
    {
        public string name;
        public int pos;
        public List<silent_Method> methods;
        public List<silent_ClassVariable> members;
        public silent_Constructor constructor;
    }

    struct silent_Variable
    {
        public string name;
        public Types variableType;
        public string data;
        public int pos;
    }

    struct silent_ClassVariable
    {
        public int pos;
        public string name;
        public Types variableType;
        public string data;
        public silent_Class owner;
        public AccessModifiers accessModifier;
    }   

    struct silent_Expression
    {
        string value1;
        string value2;
        expressionOperation operation;
    }

    struct silent_Array
    {
        public string name;
        public List<silent_Variable> variables;
        public int Length;
    }

    struct silent_Struct
    {
        public int pos;
        public string name;
        public List<silent_Variable> variables;
        public List<silent_Array> arrays;
    }

    struct silent_Constructor
    {
        public int pos;
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
        public int pos;
    }

    struct silent_Method
    {
        public AccessModifiers access;
        public string name;
        public List<silent_Expression> expressions;
        public Types returnType;
        public silent_Variable returnValue;

        public List<silent_Variable> localVariables;
        public List<silent_Struct> localStructs;
        public List<silent_Array> localArrays;
        public int pos;
    }

    enum expressionOperation
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Equal,
        Assign
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
