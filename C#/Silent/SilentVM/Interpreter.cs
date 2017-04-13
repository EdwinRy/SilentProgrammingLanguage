using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SilentVM;

namespace SilentVM
{
    class Interpreter
    {

        public bool running;
        private int programCounter;
        private Stack stack;
        private string[] source;
        string[] instructionLine;
        int instruction;
        string data;

        public void Interpret(string byteCode)
        {
            source = byteCode.Split('\n');

            stack = new Stack();
            running = true;
            programCounter = -1;

            while (running)
            {
                Fetch();
                Decode();
                Execute(instruction, data);
            }

        }

        private void Fetch()
        {
            programCounter += 1;
        }

        private void Decode()
        {
            instructionLine = source[programCounter].Split(' ');
            instruction = int.Parse(instructionLine[0]);
            data = instructionLine[1];
        }

        private void Execute(int instruction, string data)
        {
            switch (instruction)
            {
                case 0:
                    stack.Clear();
                    running = false;
                    //Environment.Exit(1);
                    break;
                case 1:
                    stack.Clear();
                    break;
                case 2:
                    programCounter = int.Parse(data);
                    break;
                case 3:
                    stack.Store(data);
                    break;
                case 4:
                    stack.Push(data);
                    break;
                case 5:
                    stack.Pop();
                    break;
                case 6:
                    stack.Add();
                    break;
                case 7:
                    stack.Subtract();
                    break;
                case 8:
                    stack.Multiply();
                    break;
                case 9:
                    stack.Divide();
                    break;
            }
        }

        private enum instructions : byte
        {
            Halt,
            Clear,
            GoTo,
            Store,
            Push,
            Pop,
            Add,
            Subtract,
            Multiply,
            Divide
        }
    }
}
