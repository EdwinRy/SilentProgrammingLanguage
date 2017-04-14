using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentVM
{
    public class Stack
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

        //MEMORY OPERATIONS
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
        }

        public void SetAt(string data)
        {
            storage[int.Parse(data)] = memory[stackPointer];
            Pop();
        }

        public void ClearMemory()
        {
            memory.Clear();
            stackPointer = -1;
        }

        public void ClearStorage()
        {
            storage.Clear();
        }

        //MATHS OPERATIONS
        public void Add()
        {
            string result = (
                float.Parse(memory[stackPointer - 1]) +
                float.Parse(memory[stackPointer])).ToString();

            Pop();Pop();
            Push(result);
        }

        public void Subtract()
        {
            string result = (
                float.Parse(memory[stackPointer - 1]) -
                float.Parse(memory[stackPointer])).ToString();

            Pop(); Pop();
            Push(result);
        }

        public void Multiply()
        {
            string result = (
                float.Parse(memory[stackPointer - 1]) *
                float.Parse(memory[stackPointer])).ToString();

            Pop(); Pop();
            Push(result);
        }

        public void Divide()
        {
            string result = (
                float.Parse(memory[stackPointer - 1]) /
                float.Parse(memory[stackPointer])).ToString();

            Pop(); Pop();
            Push(result);
        }
        
        //LOGICAL OPERATIONS
        public void SmallerThan()
        {
            string leftSide = memory[stackPointer - 1];
            string rightSide = memory[stackPointer];

            Pop(); Pop();

            if (float.Parse(leftSide) < float.Parse(rightSide))
            {
                Push("1");
            }

            else
            {
                Push("0");
            }
        }

        public void LargerThan()
        {
            string leftSide = memory[stackPointer - 1];
            string rightSide = memory[stackPointer];

            Pop(); Pop();

            if (float.Parse(leftSide) > float.Parse(rightSide))
            {
                Push("1");
            }

            else
            {
                Push("0");
            }
        }

        public void Equal()
        {
            string leftSide = memory[stackPointer - 1];
            string rightSide = memory[stackPointer];

            Pop(); Pop();

            if (float.Parse(leftSide) == float.Parse(rightSide))
            {
                Push("1");
            }

            else
            {
                Push("0");
            }
        }

    }
}
