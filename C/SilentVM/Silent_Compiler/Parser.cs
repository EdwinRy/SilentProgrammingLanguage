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

        public void Parse(List<Tokens> tokens, List<string> values)
        {
            this.tokens = tokens;
            this.values = values;

        }

        private List<silent_Function> SortFunctions()
        {
            List<silent_Function> functions = new List<silent_Function>();


            return functions;
        }

        private void SortVariables()
        {
            List<silent_Variable> variables = new List<silent_Variable>();

        }

    }

    struct silent_Namespace
    {
        string name;
        List<silent_Function> functions;
        List<silent_Variable> variables;
        List<silent_Class> classes;
    }

    struct silent_Class
    {
        string name;
    }

    struct silent_Variable
    {
        string name;
        Types variableType;
        string data;
    }

    struct silent_Expression
    {
        string name;
        List<Tokens> tokens;
        List<string> values;
    }

    struct silent_Array
    {
        string name;
        List<silent_Variable> variables;
        int Length;
    }

    struct silent_Struct
    {
        string name;
        List<silent_Variable> variables;
        List<silent_Array> arrays;
    }

    struct silent_Function
    {
        string name;
        List<silent_Expression> expressions;
        silent_Variable returnType;

        List<silent_Variable> localVariables;
        List<silent_Struct> localStructs;
        List<silent_Array> localArrays;
    }

    struct silent_Method
    {
        AccessModifiers access;
        string name;
        List<silent_Expression> expressions;
        silent_Variable returnType;

        List<silent_Variable> localVariables;
        List<silent_Struct> localStructs;
        List<silent_Array> localArrays;
    }

    enum Types
    {
        Char,
        Integer,
        Float,
        Null
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
