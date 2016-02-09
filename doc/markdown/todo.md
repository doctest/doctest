##TODO

move to dashes from underscores for command line arguments

ability to not run tests at all

use a pool for the allocations because 1k+ allocs before entering main() can be heavy

move to RAII instead of atexit() bullshit (and maybe stop this void* BS)

more aggressive gcc/clang warnings! http://meetingcpp.com/tl_files/2014/talks/robust_code.pdf





__attribute((unused))__ for tests when disabled globally (or made as templates that never get instantiated)








the CHECK() macro
    OPTIONAL!!! break on failed CHECK() macros in doctest

a message macro

squash git history for version 1 (after the CHECK() macro is done and some reporting is present) and recreate branches

timing reports of tests, duration restrictions, kill of longer than, etc...

look at catch command line options

defense of macros in testing frameworks: http://accu.org/var/uploads/journals/Overload125.pdf

tests/examples folders separate

profile doctest vs Catch
    compile time
    startup time

error reporting
- MSVC/IDE integration
- xml
- make a compact reporter (for within ide-s - just the file and line number)
- streaming reporters???

signal handling for unix: http://www.cplusplus.com/reference/csignal/signal/

test execution in separate processes
    UNIX - fork()
    process stuff under windows
        maybe call itself? with the range functionality? better not...
        http://msdn.microsoft.com/en-us/library/windows/desktop/ms682512(v=vs.85).aspx
        http://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx

CI
    travis
        valgrind
        clang
        osx

https://gitter.im/

use https://github.com/skywinder/github-changelog-generator

THINK OF IN THE LONG TERM

think about removing the custom exception catcher/test runner function (testExecutionWrapper)

sorting the test order (by file, by test suite, or both, ASC/DESC...)

trim "\"" from test/fixture/testsuite names?

- depends only on profiling different schemas
    
hierarchical test suites? using a stack for the pushed states

error checking of parameters (if the range is valid (first <= last), etc)

ability to transfer/copy registered functions from one shared object to another so they are put in one set and duplicates are filtered

to make it C compatible:

- WHY DON'T YOU USE C++?

- make a DOCTEST_C_INTERFACE macro

- wrap all casts

- change c standard library includes

- use the struct keyword everywhere

- remove counter definitions from for loops

- remove exception catching

- migrate to pointers from references

- remove the fixture macro

- the CHECK macro... will need CHECK_EQUAL and all that bullshit because it cannot be implemented in plain C...

- remove namespaces (wrap in a macro every doctest function declaration/definition/call for a "doctest_" prefix)

documentation

- whats the library's main purpose

- warning free even with the most aggressive options for all 3 major compilers

- mocking features are not necessary because they are orthogonal to testing and a different third party library may be used for that (google
 mock)
 
- document what has been taken from Catch

- document how to use spaces for filters in the comma separated list (using "")

- tests are ran serially

- document all the options

- library is easily breakable if __counter__ is not defined (a test in a header and a test in a cpp on the same line as the header) - will result in compilation error


