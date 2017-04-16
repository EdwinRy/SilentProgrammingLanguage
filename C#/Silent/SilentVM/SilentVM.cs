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
        private List<string> byteCode = new List<string>();

        private Interpreter interpreter;

        private List<SourceCode> sources = new List<SourceCode>();

        public SilentVM()
        {
            interpreter = new Interpreter();
        }

        public void LoadScript(string filePath)
        {
            PrepareScript(filePath);

            for(int i = 0; i < sources.ToArray().Length; i++)
            {
                if (sources[i].startingPoint)
                {
                    //move the starting bytecode to the front
                    sources.Insert(0, sources[i]);
                    sources.RemoveAt(i+1);
                }
            }

            //Add sorting out multiple algorithms

            for(int i = 1; i < sources.ToArray().Length; i++)
            {
                for(int x = 0; x < sources[i].source.Length; x++)
                {

                }
            }
            
        }

        public void ExecuteScript(string filePath)
        {
            interpreter.Interpret(File.ReadAllLines(filePath));
        }

        private void PrepareScript(string filePath)
        {
            SourceCode newSource = new SourceCode();
            newSource.source = File.ReadAllLines(filePath);

            for(int i = 0; i < newSource.source.Length; i++)
            {
                if(newSource.source[i].StartsWith(".n"))
                {
                    newSource.name = newSource.source[i].Split(' ')[1];
                }

                else if (newSource.source[i] == ".s")
                {
                    newSource.startingPoint = false;
                }

                else if (newSource.source[i].StartsWith(".u"))
                {
                    PrepareScript(newSource.source[i].Split(' ')[1]);
                }

                else
                {
                    sources.Add(newSource);
                    break;
                }

            }

        }

        private void AddModule(string path)
        {

        }

        public void Start()
        {
            interpreter.Interpret(sources[0].source);
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

    struct SourceCode
    {
        public string[] source;
        public bool startingPoint;
        public string name;
    }

}
