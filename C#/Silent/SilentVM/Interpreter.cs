using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SilentVM;

namespace SilentVM
{
    public class Interpreter
    {
        //State of execution
        public bool running;
        private int programCounter;

        //Memory
        public Stack stack;
        public Dictionary<string,Action> libraries = new Dictionary<string, Action>();

        //Script
        private string[] source;
        string[] instructionLine;

        //Current state
        int instruction;
        string data;

        public Interpreter()
        {
            stack = new Stack();
            running = true;
            programCounter = -1;
        }

        public void Interpret(string[] byteCode)
        {
            source = byteCode;

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

            if (programCounter > source.Length)
            {
                Environment.Exit(1);
            }
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

                //System

                case (int)instructions.Halt:
                    stack.ClearMemory();
                    stack.ClearStorage();
                    running = false;
                    //Environment.Exit(1);
                    break;

                case (int)instructions.ClearMemory:
                    stack.ClearMemory();
                    break;

                case (int)instructions.ClearStorage:
                    stack.ClearStorage();
                    break;

                case (int)instructions.GoTo:
                    programCounter = int.Parse(data);
                    break;

                case (int)instructions.Call:
                    libraries[data].Invoke();
                    break;

                //Memory

                case (int)instructions.Push:
                    stack.Push(data);
                    break;

                case (int)instructions.Pop:
                    stack.Pop();
                    break;

                case (int)instructions.PushBack:
                    stack.PushBack(data);
                    break;

                case (int)instructions.PopBack:
                    stack.PopBack();
                    break;

                case (int)instructions.Store:
                    stack.Store(data);
                    break;

                case (int)instructions.StoreTop:
                    stack.StoreTop();
                    break;

                case (int)instructions.StoreBack:
                    stack.StoreBack();
                    break;

                case (int)instructions.SetAt:
                    stack.SetAt(data);
                    break;

                case (int)instructions.Load:
                    stack.Load(data);
                    break;

                case (int)instructions.LoadBack:
                    stack.LoadBack(data);
                    break;

                //Maths

                case (int)instructions.Add:
                    stack.Add();
                    break;

                case (int)instructions.Subtract:
                    stack.Subtract();
                    break;

                case (int)instructions.Multiply:
                    stack.Multiply();
                    break;

                case (int)instructions.Divide:
                    stack.Divide();
                    break;

                //Logic

                case (int)instructions.SmallerThan:
                    stack.SmallerThan();
                    break;

                case (int)instructions.LargerThan:
                    stack.LargerThan();
                    break;

                case (int)instructions.Equal:
                    stack.Equal();
                    break;

                case (int)instructions.IfTrue:
                    IfTrue(data);
                    break;
            }
        }

        public void IfTrue(string data)
        {
            if (stack.memory[stack.stackPointer] == "1")
                programCounter = int.Parse(data);
        }

             
        private enum instructions : byte
        {
            //System operations : 5
            Halt,                           
            ClearMemory,                    
            ClearStorage,   
            GoTo,           
            Call,           

            //Memory operations : 10
            Push,           
            Pop,            
            PushBack,
            PopBack,
            Store,
            StoreTop,
            StoreBack,
            SetAt,
            Load,
            LoadBack,

            //Maths operations : 4
            Add,            
            Subtract,       
            Multiply,       
            Divide,         

            //Logical operations : 4
            SmallerThan,    
            LargerThan,     
            Equal,
            IfTrue

        } 
    }
}
