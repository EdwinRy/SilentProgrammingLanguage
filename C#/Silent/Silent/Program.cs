using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SilentVM;

namespace Silent
{
    class Program
    {

        static void Main(string[] args)
        {

            for(int i = 0; i < args.Length; i++)
            {
                if(args[i] == "-c")
                {

                }

                if(args[i] == "-i")
                {
                    SilentVM.SilentVM virtualMachine = new SilentVM.SilentVM();
                    virtualMachine.LoadScript(args[i + 1]);
                    virtualMachine.Start();
                }

                if (args[i] == "-e")
                {
                    //SilentVM.SilentVM virtualMachine = new SilentVM.SilentVM();
                    Manager man = new Manager(args[i + 1]);
                    //virtualMachine.ExecuteScript(args[i + 1]);
                    //virtualMachine.Start();
                }
            }
        }

    }

    class Manager
    {
        SilentVM.SilentVM virtualMachine;
        public Manager(string script)
        {
            virtualMachine = new SilentVM.SilentVM();
            virtualMachine.AddFunction("print", Print);
            virtualMachine.ExecuteScript(script);
            virtualMachine.Start();
        }

        public void Print()
        {
            Console.WriteLine(virtualMachine.getMemory()[0]);
        }
    }
}
