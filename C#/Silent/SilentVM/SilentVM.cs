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
        private string[] byteCode;
        private Interpreter interpreter;

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

        public void AddFunction(string functionName, Action function)
        {
            interpreter.libraries.Add(functionName, function);
        }

        public List<string> getMemory()
        {
            return interpreter.stack.memory;
        }

        public void pushData(string data)
        {
            interpreter.stack.Push(data);
        }

        public void popData()
        {
            interpreter.stack.Pop();
        }

    }
}
