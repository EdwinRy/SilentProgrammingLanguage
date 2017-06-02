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
    }

    struct silent_Variable
    {
        Types variableType;
        string data;
    }

    struct silent_Expression
    {
        List<Tokens> tokens;
        List<string> values;
    }

    struct silent_Array
    {
        List<silent_Variable> variables;
        int Length;
    }

    struct silent_Struct
    {
        List<silent_Variable> variables;
    }

    struct silent_Function
    {
        List<silent_Expression> expressions;
        silent_Variable returnType;
    }

    enum Types
    {
        Char,
        Integer,
        Float,
        Null
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
