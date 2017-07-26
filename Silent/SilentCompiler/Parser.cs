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

        //Parser object constructor - declare the program and its values
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
            Parse();
        }

        //Turn the list of tokens into silent objects
        public void Parse()
        {
            //Iterate through the list of tokens
            for (int i = 0; i < globalScope.tokens.Count; i++)
            {
                //If the current token is a decleration of a namespace
                if (globalScope.tokens[i] == Tokens.Namespace)
                {
                    //Add a namespace object to the list of namespaces
                    globalScope.namespaces.Add(PrepareNamespace(i, out i));
                }
                
                //If the current token is a decleration of a class
                else if (globalScope.tokens[i] == Tokens.Class)
                {
                    //Add a class object to the list of classes
                    globalScope.classes.Add(PrepareClass(i, out i));
                }

                //If the current token is a decleration of a function
                else if (globalScope.tokens[i] == Tokens.Function)
                {
                    //Add a function object to the list of functions
                    globalScope.functions.Add(PrepareFunction(i, out i));
                }

                else
                {
                    PrintError("Code in the global scope will not be executed");
                }
            }
        }

        silent_Expression PrepareExpression(int pos, out int position)
        {
            silent_Expression Expression = new silent_Expression();
            //Used to get the length of the scope
            int startPos = pos;
            int endPos = 0;

            position = endPos;
            return Expression;
        }

        silent_ClassVariable PrepareClassVariable(int pos, out int position)
        {
            silent_ClassVariable Variable = new silent_ClassVariable();
            //Used to get the length of the scope
            int startPos = pos;
            int endPos = 0;

            position = endPos;
            return Variable;
        }

        silent_Variable PrepareVariable(int pos, out int position)
        {
            silent_Variable Variable = new silent_Variable();
            //Used to get the length of the scope
            int startPos = pos;
            int endPos = 0;

            position = endPos;
            return Variable;
        }

        silent_Function PrepareFunction(int pos, out int position)
        {
            silent_Function Function = new silent_Function();
            //Used to get the length of the scope
            int startPos = pos;
            int endPos = 0;


            position = endPos;
            return Function;
        }

        //Sort method within a class
        silent_Method PrepareMethod(int pos, out int position)
        {
            silent_Method Method = new silent_Method();
            //Used to get the length of the scope
            int startPos = pos;
            int endPos = 0;

            //Number of scopes within the method
            int noScopes = 0;

            bool namespaceOpened = false;

            //Isolate the scope from the rest of the program
            for (int i = startPos; i < globalScope.tokens.Count; i++)
            {

                //If opening bracket appears, a new scope is created
                if (globalScope.tokens[i] == Tokens.OpenCurlyBracket)
                {
                    noScopes++;
                    namespaceOpened = true;
                }

                //If closing bracket appears, a scope is closed
                if (globalScope.tokens[i] == Tokens.CloseCurlyBracket)
                {
                    noScopes--;
                }

                //If the closing bracket closes current method scope
                if (noScopes == 0 && namespaceOpened == true)
                {
                    endPos = i + 1;

                    //Copy the code into the method object
                    for (int x = startPos; x < endPos; x++)
                    {
                        Method.tokens.Add(globalScope.tokens[x]);
                    }

                    break;
                }
            }

            position = endPos;
            return Method;
        }

        //Sort out class structure
        silent_Class PrepareClass(int pos, out int position)
        {
            silent_Class Class = new silent_Class()
            {
                members = new List<silent_ClassVariable>(),
                methods = new List<silent_Method>(),
                pos = pos
            };

            //Used to get the length of the scope
            int startPos = globalScope.pos;
            int endPos = 0;

            //Number of scopes within the class
            int noScopes = 0;

            bool namespaceOpened = false;

            //Isolate the scope from the rest of the program
            for (int i = startPos; i < globalScope.tokens.Count; i++)
            {

                //If opening bracket appears, a new scope is created
                if (globalScope.tokens[i] == Tokens.OpenCurlyBracket)
                {
                    noScopes++;
                    namespaceOpened = true;
                }

                //If closing bracket appears, a scope is closed
                if (globalScope.tokens[i] == Tokens.CloseCurlyBracket)
                {
                    noScopes--;
                }

                //If the closing bracket closes current class scope
                if (noScopes == 0 && namespaceOpened == true)
                {
                    endPos = i + 1;

                    //Copy the code into the class object
                    for (int x = startPos; x < endPos; x++)
                    {
                        Class.tokens.Add(globalScope.tokens[x]);
                    }

                    break;
                }
            }

            //If the token after the class token is a value, it becomes the class's name
            if (Class.tokens[1] == Tokens.Value)
            {
                //Declare the class's name
                Class.name = this.values[CountVal(Class.pos)];
            }

            if (Class.tokens[1] != Tokens.Value || Class.tokens[2] != Tokens.OpenCurlyBracket)
            {
                PrintError("class declaration invalid");
            }


            //Parse
            //Iterate through the list of tokens
            for (int i = startPos; i < Class.tokens.Count; i++)
            {
                //If the current token is a decleration of a method
                if (Class.tokens[i] == Tokens.Method)
                {
                    Class.methods.Add(PrepareMethod(i, out i));
                }

                //If the current token is a decleration of a variable
                if (Class.tokens[i] == Tokens.Variable)
                {
                    Class.members.Add(PrepareClassVariable(i, out i));
                }

                else
                {
                    PrintError("Instructions directly in the class scope will not be executed");
                }
            }

            position = endPos;
            return Class;

        }

        //Sort namespace in the global scope
        silent_Namespace PrepareNamespace(int pos, out int position)
        {
            //Create new namespace
            silent_Namespace Namespace = new silent_Namespace()
            {
                tokens = new List<Tokens>(),
                namespaces = new List<silent_Namespace>(),
                functions = new List<silent_Function>(),
                //Assign the namespace position in the code
                pos = pos
            };

            //Used to get the length of the scope
            int startPos = pos;
            int endPos = 0;

            //Number of scopes within the namespace
            int noScopes = 0;

            bool namespaceOpened = false;

            //Isolate the scope from the rest of the program
            for(int i = startPos; i < globalScope.tokens.Count; i++)
            {

                //If opening bracket appears, a new scope is created
                if (globalScope.tokens[i] == Tokens.OpenCurlyBracket)
                {
                    noScopes++;
                    namespaceOpened = true;
                }

                //If closing bracket appears, a scope is closed
                if(globalScope.tokens[i] == Tokens.CloseCurlyBracket)
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
                        Namespace.tokens.Add(globalScope.tokens[x]);
                    }
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
            for (int i = startPos; i < Namespace.tokens.Count; i++)
            {
                //If the current token is a decleration of a namespace
                if (Namespace.tokens[i] == Tokens.Namespace)
                {
                    //Add a namespace object to the list of namespaces
                    Namespace.namespaces.Add(PrepareNamespace(i , out i));
                }

                //If the current token is a decleration of a class
                if (Namespace.tokens[i] == Tokens.Class)
                {
                    //Add a class object to the list of classes
                    Namespace.classes.Add(PrepareClass(i, out i));
                }

                //If the current token is a decleration of a function
                else if (Namespace.tokens[i] == Tokens.Function)
                {
                    //Add a function object to the list of functions
                    Namespace.functions.Add(PrepareFunction(i, out i));
                }

                else
                {
                    PrintError(("Instructions can't be executed within an uncallable scope: "+Namespace.name));
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
        public int varNO;
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
        public List<Tokens> tokens;
        List<silent_Variable> variables;
        List<silent_SubExpression> brokenExpressions;
    }

    struct silent_SubExpression
    {
        silent_Variable value1;
        silent_Variable value2;
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

        public List<Tokens> tokens;
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
        Public,
        Protected
    }
}
