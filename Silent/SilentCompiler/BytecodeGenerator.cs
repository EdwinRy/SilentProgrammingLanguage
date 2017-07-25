using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentCompiler
{
    class BytecodeGenerator
    {
        Parser parser;

        public BytecodeGenerator(Parser parserobj)
        {
            this.parser = parserobj;
        }
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
