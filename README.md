# McDonnell-OOP

2/22
- So far, I am still compiling in C using GCC.  I have only taken out a variable that was explicity declared twiced in the build, and put one instance in the rogue.h header.  I also added file guards in rogue.h to ensure that the contents of rogue.h cannot be defined more than once, as I was receiving an error about variables from rogue.h being redeclared in every object file during compilation.
