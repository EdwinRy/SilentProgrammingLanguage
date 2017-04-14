using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentVM
{
    class Stack
    {
        public int stackPointer;
        public List<string> memory;
        private List<string> storage;

        public Stack()
        {
            memory = new List<string>();
            storage = new List<string>();
            stackPointer = -1;
        }

        public void Push(string data)
        {
            memory.Add(data);
            stackPointer += 1;
        }

        public void Pop()
        {
            memory.RemoveAt(stackPointer);
            stackPointer -= 1;
        }

        public void Store(string data)
        {
            storage.Add(data);
            Pop();
        }

        public void Add()
        {
            string result = (int.Parse(memory[stackPointer]) + 
                int.Parse(memory[stackPointer-1])).ToString();

            Pop();Pop();
            Push(result);
            Console.WriteLine(memory[stackPointer]);
        }

        public void Subtract()
        {
            string result = (int.Parse(memory[stackPointer - 1]) -
                  int.Parse(memory[stackPointer])).ToString();

            Pop(); Pop();
            Push(result);
        }

        public void Multiply()
        {
            string result = (int.Parse(memory[stackPointer - 1]) *
                  int.Parse(memory[stackPointer])).ToString();

            Pop(); Pop();
            Push(result);
        }

        public void Divide()
        {
            string result = (int.Parse(memory[stackPointer - 1]) /
                  int.Parse(memory[stackPointer])).ToString();

            Pop(); Pop();
            Push(result);
        }

        public void Clear()
        {
            memory.Clear();
            stackPointer = 0;
        }
    }
}
