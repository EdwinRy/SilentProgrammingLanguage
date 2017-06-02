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
}
