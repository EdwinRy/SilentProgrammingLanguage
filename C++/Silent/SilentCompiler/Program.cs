using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentCompiler
{



    class Program
    {
        static void Main(string[] args)
        {
            Compiler compiler = new Compiler();
            compiler.compile(args[0]);
        }
    }

    class Compiler
    {
        public void compile(string filepath)
        {
            string source = File.ReadAllText(filepath);
        }

        public void Tokenize(string SourceCode)
        {



        }
    }
}
