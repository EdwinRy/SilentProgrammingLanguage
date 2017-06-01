﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SilentVM;
using System.IO;
using SilentCompiler;

namespace Silent
{
    class Program
    {

        static void Main(string[] args)
        {
            SilentCompiler.SilentCompiler compiler = new SilentCompiler.SilentCompiler();
            string source = File.ReadAllText("test.txt");
            compiler.Compile(source);
            Console.ReadLine();
            
        }

    }

}
