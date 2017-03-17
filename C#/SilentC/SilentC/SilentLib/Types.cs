using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SilentC.SilentLib.Types
{
    struct Integer
    {
        int value;

        void ToString()
        {
            Console.WriteLine(value);
        }

    }

    struct String
    {
        string value;

        void ToString()
        {
            Console.WriteLine(value);
        }
    }

    struct Boolean
    {
        bool value;

        void ToString()
        {
            Console.WriteLine(value);
        }

    }

    struct Function
    {
        Integer[] ScopeIntegers;
        String[] ScopeStrings;
        Boolean[] ScopeBooleans;

        void ToString()
        {
            Console.WriteLine("Function at:" + &this );
        }
    }

    struct Class
    {
        Integer[] ScopeIntegers;
        String[] ScopeStrings;
        Boolean[] ScopeBooleans;

        Function[] ScopeFunctions;
    }




}
