# McDonnell-OOP

2/22
- So far, I am still compiling in C using GCC.  I have only taken out a variable that was explicity declared twiced in the build, and put one instance in the rogue.h header.  I also added file guards in rogue.h to ensure that the contents of rogue.h cannot be defined more than once, as I was receiving an error about variables from rogue.h being redeclared in every object file during compilation.

4/26
- Now compiling in C++ using G++.  Going through each function and putting parameters between the parantheses in function definitions.  I often have to put a declaration of the function in each individual file that calls it in order to make it compile.  Quick fix was done on macros for function definitions of "attach" and "detach", thanks to Dr. Sutton.  Overall technique so far: compile in linux terminal using make, find first error, find file to locate the origin of that error, completely fix that file until it compiles on its own, which requires fixing any functions in other files that that file calls.

4/29
- All functions have been converted to modern C++ as far as paramters and return types are concerned.

5/1
- Rogue now completely compiles in C++.  Difficult to say whether the functionality remains the same or if I butchered the whole game.  I never understood how to play the game in the first place.  But the important thing is that it compiles.  But also, it compiles... yeah, IT COMPILES.

5/2
  - Rogue still completely compiles using g++.  I have now gotten rid of all warnings as well.  I mostly had to do some type casting, 90% of the warnings were string constants being treated as char*, so I cast those as char* for individual purposes.  I feel this probably took longer but overall was less difficult than changing every function to accept const char*, and then I would have to overload certain functions to accept both char* and const char* where applicable.  Also, there were some warnings with integers being compared to unsigned ints, so on and so forth.  Overall, I just wanted a clean screen when someone presses "make" on the game.
