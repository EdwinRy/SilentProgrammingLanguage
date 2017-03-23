using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentC
{
    class Program
    {
        static void Main(string[] args)
        {
            string fileToCompile;


            for(int i = 0; i < args.Count(); i++)
            {
                if(args[i] == "-c")
                {
                    fileToCompile = args[i + 1];
                }
            }

        }

        static void Main()
        {
            Console.WriteLine("-c compiles the file and returns the bytecode file of the same name as the input file");
            Console.WriteLine("-o set the output name of the generated file");
            Console.WriteLine("Pass an argument of the path to the main file to be compiled");
        }

        static void Compile(string filePath)
        {

        }

        /*
        static void Compile(string )
        {

        }
        */
    }
}
