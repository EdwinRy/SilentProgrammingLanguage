# Silent Programming Language
A fast, general-purpose programming language 

## Sample syntax
    func int64 fibonacci(int64 x)
    {
        if(x > 2)
        {
            return fib(x - 1) + fib(x - 2); 
        }
        
        return 1;
    }

    func int64 main()
    {
        int64 x = fibonacci(40);
    }

# Installation
    cd SilentProgrammingLanguage
    make compiler
    make vm