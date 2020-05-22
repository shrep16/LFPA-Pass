# LFPA-Pass
Liveness based Flow sensitive Pointer Analysis (LFPA) Pass is a modified version of pointer analysis that performs flow sensitive pointer analysis. It combines strong liveness and pointer analysis. It can be also extended to context sensitivity(LFCPA). It improves on scalability factors. This pass is implemented in LLVM.

* Supports : Global and Local Variables (pointers); all control flow constructs (if-else, while/for)
* Future Scope : Structures and Arrays
* This is an out of tree pass
* Supports C and C++ input test files

Directions for running :
1. Install llvm and cmake
2. Set the paths accordingly in the CMakeLists.txt and test(bash file)
3. Write your tests and edit the path for same in test(bash file)
4. ./test
5. In order to print the pointer analysis output and strong liveness analysis output, uncomment the comments in the respective functions

Utilities created:
1. May points to
2. Must points to
3. Does not points to

Directions for writing test cases :
1. Make sure you include the header file -myHeader.h

