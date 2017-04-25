using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace SilentCompiler
{
    public class SilentCompiler
    {

        string[] Source;
        List<string> ByteCode = new List<string>();
        GlobalScope MainScope = new GlobalScope();

        public SilentCompiler()
        {

        }

        public void Compile(string filePath)
        {
            string source = File.ReadAllText(filePath);

            Source = source.Split(' ');

            List<string> instructions = new List<string>();

            for(int i = 0; i < source.Length; i++)
            {

                int startIndex = -1;
                int endIndex = -1;

                if(source[i] == '#')
                {
                    startIndex = i;
                }

                if (source[i] == ';' && startIndex != -1)
                {
                    endIndex = i;
                }

                if(startIndex != -1 && endIndex != -1)
                {
                    ExecuteCompilerInstruction(source.Substring(startIndex, endIndex - startIndex));
                    startIndex = -1;
                    endIndex = -1;
                }

            }
        }

        public void ExecuteCompilerInstruction(string instruction)
        {
            if (instruction.StartsWith("#include"))
            {
                string include = instruction.Split(' ')[1].Remove(-1);
            }
        }

        public void Structure()
        {

        }
    }

    public struct GlobalScope
    {
        List<Namespace> Namespaces;
        List<Class> GlobalClasses;
        List<Function> GlobalFunctions;

    }

    public struct Namespace
    {
        List<Class> Classes;
        List<Function> Functions;
    }

    public struct Class
    {
        string ClassName;
        AccessModifiers AccessModifier;
        List<Variable> ClassMembers;
        List<Method> Methods;
    }

    public struct Method
    {
        string MethodName;
        Class owner;
        AccessModifiers AccessModifier;
        List<Variable> variables;
        List<string> Instructions;
        List<Variable> Parameters;
        Variable ReturnValue;
    }

    public struct Function
    {
        string FunctionName;
        PrimitiveDataType ReturnType;
        List<Variable> variables;
        List<string> Instructions;
        List<Variable> Parameters;
        Variable ReturnValue;
    }

    public struct Struct
    {
        string StructName;
        List<Variable> variables;
    }

    public struct Array
    {
        string ArrayName;
        AccessModifiers AccessModifier;
        PrimitiveDataType dataType;

        List<Variable> variables;
    }

    public struct Variable
    {
        string VariableName;
        AccessModifiers AccessModifier;
        PrimitiveDataType dataType;

        string Data;
    }

    public enum PrimitiveDataType
    {
        Character,
        Integer,
        Float
    }

    public enum AccessModifiers
    {
        Public,
        Private
    }
}
