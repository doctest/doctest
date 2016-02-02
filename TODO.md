##TODO

ability to not run tests at all

use a pool for the allocations because 1k+ allocs before entering main() can be heavy

move to RAII instead of atexit() bullshit (and maybe stop this void* BS)

more aggressive gcc/clang warnings! http://meetingcpp.com/tl_files/2014/talks/robust_code.pdf





__attribute((unused))__ for tests when disabled globally





the CHECK() macro
    OPTIONAL!!! break on failed CHECK() macros in doctest

a message macro

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
    https://svn.boost.org/trac/boost/wiki/TravisCoverals
    travis
        compile/run all examples
        valgrind
            https://github.com/dlundquist/sniproxy/blob/master/.travis.yml
        clang static analysis
        coverity static analysis http://www.coverity.com/
        coverage
            https://github.com/eddyxu/cpp-coveralls
            https://github.com/kylelutz/compute
            https://github.com/apolukhin/variant/blob/travisci/.travis.yml
    msvc
        just compile all the examples
        https://github.com/mkurdej/bft/blob/develop/appveyor.yml
        https://github.com/mapbox/variant/blob/master/appveyor.yml
        https://github.com/cppformat/cppformat/blob/master/appveyor.yml

badges - https://github.com/davidsiaw/luacppinterface

https://bitdeli.com/

https://gitter.im/

http://shields.io/

THINK OF IN THE LONG TERM

think about removing the custom exception catcher/test runner function (testExecutionWrapper)

sorting the test order (by file, by test suite, or both, ASC/DESC...)

trim "\"" from test/fixture/testsuite names?

- depends only on profiling different schemas
    
hierarchical test suites? using a stack for the pushed states

error checking of parameters (if the range is valid (first <= last), etc)

ability to transfer/copy registered functions from one shared object to another so they are put in one set and duplicates are filtered

to make it C compatible:

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
















https://svn.boost.org/trac/boost/wiki/BestPracticeHandbook#a14.USERFRIENDLINESS:Considerlettingpotentialuserstryyourlibrarywithasinglemouseclick










WANDBOX TRY-IT-ONLINE

 Hi,

I've recently added a new functionality to proposed boost.di library
documentation.
After `try it online` the natural next step was to add `run this code`
functionality like in http://en.cppreference.com/w/ or
http://www.rust-lang.org/ programming language.
Powered by Wandbox and CodeMirror approach, let you play with the
particular example and switch between normal quick book programming listing
mode and the editor/compiler mode.

You can check it here:
http://krzysztof-jusiak.github.io/di/cpp14/boost/libs/di/doc/html/di/examples/hello_world.html

Just click on `Run this code` button and you will be switched to the editor
mode, where you would be able to edit the code and compile it online using
the newest version of the library.

Implementation details:
https://github.com/krzysztof-jusiak/di/blob/cpp14/doc/js/run_this_code.js

It's basically the same solution as `try it online`, just implemented in js
instead. Additionally, it also takes the newest version of the library from
github.

Minimal html:
<link rel="stylesheet" href="CodeMirror/lib/codemirror.css">
<link rel="stylesheet" href="CodeMirror/theme/zenburn.css">
<script src="CodeMirror/lib/codemirror.js"></script>
<script src="CodeMirror/mode/clike/clike.js"></script>
<script src="CodeMirror/addon/selection/active-line.js"></script>
<script src="run_this_code.js"></script>

<button class="TryItBtn" id="run_it_btn" onclick="show('
https://raw.githubusercontent.com/krzysztof-jusiak/di/cpp14/examples/hello_world.cpp)">Run
this code!<\/button>
<textarea style="display: none" id="code"></textarea>
<textarea style="display: none" id="output"></textarea>
<div id="code_listing">
//quick book programming listing here or anything else, which should be
hidden when editor mode is enabled
</div>

Let me know, what do you think.

Cheers, Kris