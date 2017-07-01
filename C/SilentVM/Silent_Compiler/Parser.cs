using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentCompiler
{
    class Parser
    {
        //List of values corresponding to the "value" tokens
        List<string> values;
        //Program in the form of tokens
        List<Tokens> program;

        //Global scope of the program
        silent_Namespace globalScope;

        //Parser object constructor - add the program and its values
        public Parser(List<Tokens> tokens, List<string> values)
        {
            //Assign local variables to the passed parameters
            this.values = values;
            this.program = tokens;

            //Create new global scope for the program
            globalScope = new silent_Namespace()
            {

                //Copy the tokens from the program to the global scope
                tokens = tokens,
                name = "meme",
                classes = new List<silent_Class>(),
                //Set the current position to 0
                pos = 0
            };

            //Start parsing the code
            Parse(globalScope);

        }

        //Turn the list of tokens into silent objects
        public void Parse(silent_Namespace scope)
        {


            //Iterate through the list of tokens
            for (int i = 0; i < scope.tokens.Count; i++)
            {
                //If the current token is a decleration of a namespace
                if (scope.tokens[i] == Tokens.Namespace)
                {
                    //Save current position in the list
                    scope.pos = i;
                    //Add a namespace object to the list of namespaces
                    scope.namespaces.Add(PrepareNamespace(scope, out i));

                }

                //If the current token is a decleration of a namespace
                if (scope.tokens[i] == Tokens.Class)
                {
                    //Save current position in the list
                    scope.pos = i;
                    //Add a namespace object to the list of namespaces
                    scope.classes.Add(PrepareClass(scope));
                    i = 0;
                }

                //If the current token is a decleration of a namespace
                else if (scope.tokens[i] == Tokens.Function)
                {
                    //Save current position in the list
                    scope.pos = i;
                    //Add a function object to the list of functions
                    scope.functions.Add(PrepareFunction(scope));
                    i = 0;
                }

                else
                {
                    PrintError("Code in the global scope will not be executed");
                }
            }
        }

        List<silent_Expression> PrepareExpression(silent_Namespace scope)
        {
            List<silent_Expression> expression = new List<silent_Expression>();

            return expression;
        }

        silent_Function PrepareFunction(silent_Namespace scope)
        {
            silent_Function function = new silent_Function();
            //function.returnType = (Types)tokens[++pos];

            return function;
        }

        silent_Class PrepareClass(silent_Namespace Namespace)
        {
            silent_Class silent_class = new silent_Class()
            {
                members = new List<silent_ClassVariable>(),
                methods = new List<silent_Method>(),
                pos = Namespace.pos
            };

            //Used to get the length of the scope
            int startPos = Namespace.pos;
            int endPos;

            //Number of scopes within the namespace
            int noScopes = 0;

            bool namespaceOpened = false;

            //Isolate the scope from the rest of the program
            for (int i = startPos; i < scope.tokens.Count; i++)
            {

                //If opening bracket appears, a new scope is created
                if (scope.tokens[i] == Tokens.OpenCurlyBracket)
                {
                    noScopes++;
                    namespaceOpened = true;
                }

                //If closing bracket appears, a scope is closed
                if (scope.tokens[i] == Tokens.CloseCurlyBracket)
                {
                    noScopes--;
                }

                //If the closing bracket closes current namespace scope
                if (noScopes == 0 && namespaceOpened == true)
                {
                    endPos = i + 1;



                    //Copy the namespace code into the namespace object
                    for (int x = startPos; x < endPos; x++)
                    {
                        silent_class.tokens.Add(scope.tokens[x]);
                    }

                    //Remove used code from the global scope as it won't be needed
                    scope.tokens.RemoveRange(startPos, (endPos - startPos));
                    break;
                }

            }

            //If the token after the namespace token is a value, it becomes the namespace's name
            if (silent_class.tokens[1] == Tokens.Value)
            {
                //Declare the namespace's name
                silent_class.name = this.values[CountVal(silent_class.pos)];
            }

            if (silent_class.tokens[1] != Tokens.Value || silent_class.tokens[2] != Tokens.OpenCurlyBracket)
            {
                PrintError("namespace declaration invalid");
            }


            return silent_class;

        }

        //Sort namespace in the global scope
        silent_Namespace PrepareNamespace(silent_Namespace scope, out int position)
        {
            //Create new namespace
            silent_Namespace Namespace = new silent_Namespace()
            {
                tokens = new List<Tokens>(),
                //Assign the namespace position in the code
                pos = scope.pos
            };

            //Used to get the length of the scope
            int startPos = scope.pos;
            int endPos = 0;

            //Number of scopes within the namespace
            int noScopes = 0;

            bool namespaceOpened = false;

            //Isolate the scope from the rest of the program
            for(int i = startPos; i < scope.tokens.Count; i++)
            {

                //If opening bracket appears, a new scope is created
                if (scope.tokens[i] == Tokens.OpenCurlyBracket)
                {
                    noScopes++;
                    namespaceOpened = true;
                }

                //If closing bracket appears, a scope is closed
                if(scope.tokens[i] == Tokens.CloseCurlyBracket)
                {
                    noScopes--;
                }

                //If the closing bracket closes current namespace scope
                if(noScopes == 0 && namespaceOpened == true)
                {
                    endPos = i + 1;



                    //Copy the namespace code into the namespace object
                    for (int x = startPos; x < endPos; x++)
                    {
                        Namespace.tokens.Add(scope.tokens[x]);
                    }

                    //Remove used code from the global scope as it won't be needed
                    //scope.tokens.RemoveRange(startPos, (endPos - startPos));
                    break;
                }
                
            }

            //If the token after the namespace token is a value, it becomes the namespace's name
            if (Namespace.tokens[1] == Tokens.Value)
            {
                //Declare the namespace's name
                Namespace.name = this.values[CountVal(Namespace.pos)];
            }

            //Check for incorrect namespace declaration
            if (Namespace.tokens[1] != Tokens.Value || Namespace.tokens[2] != Tokens.OpenCurlyBracket)
            {
                PrintError("namespace declaration invalid");
            }


            //Parse
            //Iterate through the list of tokens
            for (int i = 0; i < Namespace.tokens.Count; i++)
            {


                //If the current token is a decleration of a namespace
                if (scope.tokens[i] == Tokens.Namespace)
                {
                    //Save current position in the list
                    scope.pos = i;
                    //Add a namespace object to the list of namespaces
                    scope.namespaces.Add(PrepareNamespace(scope, out i));

                }

                //If the current token is a decleration of a namespace
                if (Namespace.tokens[i] == Tokens.Class)
                {
                    //Save current position in the list
                    Namespace.pos = i;
                    //Add a namespace object to the list of namespaces
                    scope.classes.Add(PrepareClass(scope));
                    i = 0;
                }

                //If the current token is a decleration of a namespace
                else if (scope.tokens[i] == Tokens.Function)
                {
                    //Save current position in the list
                    scope.pos = i;
                    //Add a function object to the list of functions
                    scope.functions.Add(PrepareFunction(scope));
                    i = 0;
                }

                else
                {
                    PrintError("Instructions can't be executed within an uncallable scope");
                }
            }

            Namespace.pos = startPos;
            position = endPos;
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

        void PrintError(string message)
        {
            Console.WriteLine(message);
            Console.ReadKey();
            Environment.Exit(0);
        }


    }

   

    struct silent_Namespace
    {
        public string name;
        public int pos;
        public List<silent_Class> classes;
        public List<Tokens> tokens;
        public List<silent_Namespace> namespaces;
        public List<silent_Function> functions;
    }

    struct silent_Class
    {
        public string name;
        public int pos;
        public List<silent_Method> methods;
        public List<silent_ClassVariable> members;
        public silent_Constructor constructor;
        public List<Tokens> tokens;
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
        List<silent_Expression> expressions;
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
