## Floaty
---
Floaty is a library for fast convertion and verification for the following:
* Converting a binary string to IEEE-754 32-bit floating point number
* Converting an IEEE-754 32-bit floating point number in string format to its binary representation in string format
* Verifying a binary 32-bit string
* Verifying a floating point number in string format
---
**Code structure and expected behaviour**
*floaty
    * include/floaty
        * converter32.h - contains headers that are exposed publicly
    * libs
        * . - empty for now, will probably have google benchmark lib
    * src
        * converter32.cpp - main code for convertion operations
        * playground.cpp - code which enables user to try out convertions in the terminal
    * tests
        * data - folder for keeping various edge cases for testing
        * case_generators.cpp - multithreaded generators which generate cases for testing based on various probability distributions
        * case_generators.h - declarations and documentation for case_generators.cpp
        * case_verifiers.cpp - code which performs verification for generated cases and cases stored in data folder
        * case_verifiers.h - declarations and documentation for case_verifiers.h
    * Makefile - used to run tests, format code and enable try out convertions in the terminal 

When in playground mode, user should be able to choose which convertion he/she wants, and then input cases.
Cases will be first verified, and if verification passes, the user should receive convertions for their inputs.
When testing, the user should be able to see various information about the testing process.
The following information included, but not limited to:
* how many cases loaded
* whether cases loaded in data passed
* how many cases were generated
* whether generated case distrubution is 50:50 good bad (only for input verifiers)
* whether converters produce the same outcome as stl/other library implementations  
* Final output whether tests have passed for each feature/function.  
--- 
**BUILDING**
Navigate to floaty folder from root directory "cd floaty"
* to run tests type in "Make test"
* to run playground type in "Make run" 

**WARNING**: this compiles with c++17 standard, so if it is unavailable to you, the only way 
to change it is to go into the Makefile and specify your desired standard.
---
**Rubic Points Adddressed**

The project demonstrates an understanding of C++ functions and control structures.
* most of the code is clearly organized into funcitons/classes. For example floaty/tests/case_generatos.cpp/.h

The project reads data from a file and process the data, or the program writes data to a file.
* floaty/tests/case_verifiers.cpp LINE 88 (_load_edge_cases)

The project accepts user input and processes the input.
* floaty/src/playground.cpp accepts user input and processes it in several parts

The project uses Object Oriented Programming techniques.  
* floaty/tests/case_verifiers.cpp/h uses OOP
* floaty/tests/case_generators.cpp/h uses OOP 

Classes use appropriate access specifiers for class members.  
* floaty/tests/case_verifiers.cpp/h uses public, protected and private members explicitly 
* floaty/tests/case_generators.cpp/h uses public, protected and private members explicitly 

Classes abstract implementation details from their interfaces.  
* floaty/tests/case_verifiers.cpp/h documentations and declarations in header, logic in source code
* floaty/tests/case_generators.cpp/h documentations and declarations in header, logic in source code

Classes follow an appropriate inheritance hierarchy.
Derived class functions override virtual base class functions.
Overloaded functions allow the same function to operate on different parameters.
* floaty/tests/case_verifiers.cpp/h  all verifiers inherit from Abstract base class and override virtual methods
* floaty/tests/case_generators.cpp/h  all generators inherit from Abstract base class and override virtual methods

Templates generalize functions in the project.  
* floaty/tests/case_verifiers.cpp/h abstract base class is a template class 
* floaty/tests/case_generators.cpp/h abstract base class is a template class 

The project makes use of references in function declarations.
* floaty/tests/case_generators.cpp LINE 56 (_generate) uses 2 reference parameters
* floaty/tests/case_generators.cpp LINE 99 (_gen_case) uses 2 reference parameters
The project uses destructors appropriately.
* floaty/tests/case_verifers.h LINE  29. Base class uses destructor which is propagated through child classes
The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
* float/tests/case_verifiers.cpp, each derived class has it's own constructor which initializes new generator on entering automatic scope
The project uses multithreading.
* float/tests/case_generators.cpp LINE 24 (generate) the main method which launches threads, but several other methods also related to threading
---

