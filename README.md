# McDonnell-OOP

2/22
- So far, I am still compiling in C using GCC.  I have only taken out a variable that was explicity declared twiced in the build, and put one instance in the rogue.h header.  I also added file guards in rogue.h to ensure that the contents of rogue.h cannot be defined more than once, as I was receiving an error about variables from rogue.h being redeclared in every object file during compilation.

4/26
- Now compiling in C++ using G++.  Going through each function and putting parameters between the parantheses in function definitions.  I often have to put a declaration of the function in each individual file that calls it in order to make it compile.  Quick fix was done on macros for function definitions of "attach" and "detach", thanks to Dr. Sutton.  Overall technique so far: compile in linux terminal using make, find first error, find file to locate the origin of that error, completely fix that file until it compiles on its own, which requires fixing any functions in other files that that file calls.
