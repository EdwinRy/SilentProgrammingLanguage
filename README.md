# Silent Programming Language
A fast, general-purpose programming language 

## Sample syntax
```
    func int64 fibonacci(int64 x)
    {
        if(x > 2)
        {
            return fibonacci(x - 1) + fibonacci(x - 2); 
        }
        
        return 1;
    }

    func int64 main()
    {
        int64 x = fibonacci(40);
    }
```
# Installation
    cd SilentProgrammingLanguage
    make compiler
    make vm

# Documentation
## Variables
Variables can either be primitive or of user-defined type in form of a struct or
a class, the syntax for the declaration of a variable is as follows:
```
    <type> <identifier>;
```
In case where the <type> is a primitive, variables can be initialised as they 
are declared in the following way:
```
    <type> <identifier> = <expression>;
```
Where <expression> results in a value of the matching type to the identifier

## References
The virtual machine is designed in a way that variables can be stored in either 
the stack of the virtual machine or the heap, when a variable is allocated on 
the heap the index of that object on the heap is stored on the stack as a 64 bit
 value, by default all non primitives are allocated on the heap and primitives 
on the stack, in cases where a primitive type has to be referred to by a 
reference the "ref" keyword can be used to declare a primitive variable to be 
explicitly stored on the heap, from then-on it behaves like any other object, 
the declaration of a sample reference would look like this:
```
    ref int32 x;
```
The reference declaration can also make use of the initialisation the same way 
a normal primitive does.