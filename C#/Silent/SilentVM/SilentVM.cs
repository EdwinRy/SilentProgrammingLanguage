using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentVM
{
    public class SilentVM
    {
        string[] byteCode;
        Interpreter interpreter;

        public SilentVM()
        {
            interpreter = new Interpreter();
        }

        public void LoadScript(string filePath)
        {
            byteCode = File.ReadAllLines(filePath);
        }

        public void Start()
        {
            interpreter.Interpret(byteCode);
        }

        public void Stop()
        {
            interpreter.running = false;
        }

    }
}
