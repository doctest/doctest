doctest
=======
[![Maintenance Status](http://stillmaintained.com/onqtam/doctest.png)](http://stillmaintained.com/onqtam/doctest)
[![Windows Status](http://img.shields.io/appveyor/ci/onqtam/doctest.svg?branch=master)](https://ci.appveyor.com/project/onqtam/doctest)
[![Linux Status](https://travis-ci.org/onqtam/doctest.svg?branch=master)](https://travis-ci.org/onqtam/doctest)
[![Coverage Status](https://coveralls.io/repos/onqtam/doctest/badge.png)](https://coveralls.io/r/onqtam/doctest)
[![License](http://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)
[![Github Issues](https://img.shields.io/github/issues/onqtam/doctest.svg)](http://github.com/onqtam/doctest/issues)

doctest is a c++ header only library for writing tests that is inspired by the unittest functionality in the D programming language and the docstrings in python - tests are documentation and should reside near the code they test.

WARNING! The library is under heavy development and is 100% not stable! Come back when this message is deleted
==============================================================================================================

Documentation can be found [here](doc/README.md)


##TODO

make the hash-table histogram printing optional (with a parameter) and make it more beautiful

the CHECK() macro
    OPTIONAL!!! break on failed CHECK() macros in doctest

a message macro

timing reports of tests, duration restrictions, kill of longer than, etc...

uppercase vs lowercase macros - with a define!

look at catch command line options

tests/examples folders separate

profile doctest vs Catch
    compile time
    startup time

error reporting
    MSVC/IDE integration
    xml

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

THINK OF IN THE LONG TERM
    better command line names (and shorter?)
    sorting the test order (by file, by test suite, or both, ASC/DESC...)
    trim "\"" from test/fixture/testsuite names?
    keep the 2 headers (main and implementation)?
        depends only on profiling different schemas
    hierarchical test suites? using a stack for the pushed states    
    error checking of parameters (if the range is valid (first <= last), etc)
    ability to transfer/copy registered functions from one shared object to another so they are put in one set and duplicates are filtered
    to make it C compatible:
        make a DOCTEST_C_INTERFACE macro
        wrap all casts
        change c standard library includes
        use the struct keyword everywhere
        remove counter definitions from for loops
        remove exception catching
        migrate to pointers from references
        remove the fixture macro
        the CHECK macro... will need CHECK_EQUAL and all that bullshit because it cannot be implemented in plain C...
        remove namespaces (wrap in a macro every doctest function declaration/definition/call for a "doctest_" prefix)
    documentation
        whats the library's main purpose
        warning free even with the most aggressive options for all 3 major compilers
        mocking features are not necessary because they are orthogonal to testing and a different third party library may be used for that (google mock)
        document what has been taken from Catch
        document how to use spaces for filters in the comma separated list (using "")
        tests are ran serially
        document all the options
        library is easily breakable if __counter__ is not defined (a test in a header and a test in a cpp on the same line as the header) - will result in compilation error
