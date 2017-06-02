using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using SilentCompiler;

namespace SilentCompiler
{
    class Program
    {
        static void Main(string[] args)
        {
            string source = File.ReadAllText("test1.txt");
            SilentCompiler compiler = new SilentCompiler();
            compiler.Compile(source);
            Console.ReadLine();
        }
    }
}
