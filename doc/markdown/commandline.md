## Command line

**doctest** works quite nicely without any command line options at all - but for more control a bunch are available.

**Query flags** - after the result is printed the program quits without executing any test cases (and if the framework is integrated into a client codebase which [**supplies it's own ```main()``` entry point**](main.md) - the program should check the result of ```shouldExit()``` method after calling ```run()``` on a ```doctest::Context``` object and should exit - this is left up to the user).

**Int/String options** - they require a value after the ```=``` sign - without spaces! For example: ```--order-by=rand```.

**Bool options** - they expect ```1```/```yes```/```on```/```true``` or ```0```/```no```/```off```/```false``` after the ```=``` sign - but they can also be used like flags and the ```=value``` part can be skipped - then ```true``` is assumed.  

**Filters** use wildcards for matching values - where ```*``` means "match any sequence" and ```?``` means "match any one character".
To pass a pattern with an interval use ```""``` like this:  ```--test-case="*no sound*,vaguely named test number ?"```.

All the options can also be set with code (defaults/overrides) if the user [**supplies the ```main()``` function**](main.md).

| Query Flags | Description |
|:------------|-------------|
| ```-?``` &nbsp;&nbsp;&nbsp; ```--help``` ```-h``` | Prints a help message listing all these flags/options |
| ```-v``` &nbsp;&nbsp;&nbsp; ```--version``` | Prints the version of the **doctest** framework |
| ```-c``` &nbsp;&nbsp;&nbsp; ```--count``` | Prints the number of test cases matching the current filters (see below) |
| ```-ltc``` ```--list-test-cases``` | Lists all test cases by name which match the current filters (see below) |
| ```-lts``` ```--list-test-suites``` | Lists all test suites by name which have at least one test case matching the current filters (see below) |
| ```-lr``` ```--list-reporters``` | Lists all registered [**reporters**](reporters.md) |
| **Int/String Options** | <hr> |
| ```-tc``` &nbsp; ```--test-case=<filters>``` | Filters test cases based on their name. By default all test cases match but if a value is given to this filter like ```--test-case=*math*,*sound*``` then only test cases who match at least one of the patterns in the comma-separated list with wildcards will get executed/counted/listed |
| ```-tce``` ```--test-case-exclude=<filters>``` | Same as the ```-test-case=<filters>``` option but if any of the patterns in the comma-separated list of values matches - then the test case is skipped |
| ```-sf``` &nbsp; ```--source-file=<filters>``` | Same as ```--test-case=<filters>``` but filters based on the file in which test cases are written |
| ```-sfe``` ```--source-file-exclude=<filters>``` | Same as ```--test-case-exclude=<filters>``` but filters based on the file in which test cases are written |
| ```-ts``` &nbsp; ```--test-suite=<filters>``` | Same as ```--test-case=<filters>``` but filters based on the test suite in which test cases are in |
| ```-tse``` ```--test-suite-exclude=<filters>``` | Same as ```--test-case-exclude=<filters>``` but filters based on the test suite in which test cases are in |
| ```-sc``` &nbsp; ```--subcase=<filters>``` | Same as ```--test-case=<filters>``` but filters subcases based on their names |
| ```-sce``` ```--subcase-exclude=<filters>``` | Same as ```--test-case-exclude=<filters>``` but filters based on subcase names |
| ```-r``` ```--reporters=<filters>``` | List of [**reporters**](reporters.md) to use (default is ```console```) |
| ```-o``` &nbsp; ```--out=<string>``` | Output filename |
| ```-ob``` &nbsp; ```--order-by=<string>``` | Test cases will be sorted before being executed either by **the file in which they are** / **the test suite they are in** / **their name** / **random**. The possible values of ```<string>``` are ```file```/```suite```/```name```/```rand```. The default is ```file```. **NOTE: the order produced by the ```file```, ```suite``` and ```name``` options is compiler-dependent and might differ depending on the compiler used.** |
| ```-rs``` &nbsp; ```--rand-seed=<int>``` | The seed for random ordering |
| ```-f``` &nbsp;&nbsp;&nbsp; ```--first=<int>``` | The **first** test case to execute which passes the current filters - for range-based execution - see [**the example python script**](../../examples/range_based_execution.py) |
| ```-l``` &nbsp;&nbsp;&nbsp; ```--last=<int>``` | The **last** test case to execute which passes the current filters - for range-based execution - see [**the example python script**](../../examples/range_based_execution.py) |
| ```-aa``` &nbsp; ```--abort-after=<int>``` | The testing framework will stop executing test cases/assertions after this many failed assertions. The default is 0 which means don't stop at all. Note that the framework uses an exception to stop the current test case regardless of the level of the assert (```CHECK```/```REQUIRE```) - so be careful with asserts in destructors... |
| ```-scfl``` ```--subcase-filter-levels=<int>``` | Apply subcase filters only for the first ```<int>``` levels of nested subcases and just run the ones nested deeper. Default is a very high number which means *filter any subcase* |
| **Bool Options** | <hr> |
| ```-s``` &nbsp;&nbsp;&nbsp; ```--success=<bool>``` | To include successful assertions in the output |
| ```-cs``` &nbsp; ```--case-sensitive=<bool>``` | Filters being treated as case sensitive |
| ```-e``` &nbsp;&nbsp;&nbsp; ```--exit=<bool>``` | Exits after the tests finish - this is meaningful only when the client has [**provided the ```main()``` entry point**](main.md)  - the program should check the ```shouldExit()``` method after calling ```run()``` on a ```doctest::Context``` object and should exit - this is left up to the user. The idea is to be able to execute just the tests in a client program and to not continue with it's execution |
| ```-d``` &nbsp; ```--duration=<bool>``` | Prints the time each test case took in seconds |
| ```-nt``` &nbsp; ```--no-throw=<bool>``` | Skips [**exceptions-related assertion**](assertions.md#exceptions) checks |
| ```-ne``` &nbsp; ```--no-exitcode=<bool>``` | Always returns a successful exit code - even if a test case has failed |
| ```-nr``` &nbsp; ```--no-run=<bool>``` | Skips all runtime **doctest** operations (except the test registering which happens before the program enters ```main()```). This is useful if the testing framework is integrated into a client codebase which has [**provided the ```main()``` entry point**](main.md) and the user wants to skip running the tests and just use the program |
| ```-nv``` &nbsp; ```--no-version=<bool>``` | Omits the framework version in the output |
| ```-nc``` &nbsp; ```--no-colors=<bool>``` | Disables colors in the output |
| ```-fc``` &nbsp; ```--force-colors=<bool>``` | Forces the use of colors even when a tty cannot be detected |
| ```-nb``` &nbsp; ```--no-breaks=<bool>``` | Disables breakpoints in debuggers when an assertion fails |
| ```-ns``` &nbsp; ```--no-skip=<bool>``` | Don't skip test cases marked as skip with a decorator |
| ```-gfl``` ```--gnu-file-line=<bool>``` | ```:n:``` vs ```(n):``` for line numbers in output (gnu mode is usually for linux tools/IDEs and is with the ```:``` separator) |
| ```-npf``` ```--no-path-filenames=<bool>``` | Paths are removed from the output when a filename is printed - useful if you want the same output from the testing framework on different environments |
| ```-nln``` ```--no-line-numbers=<bool>``` | Line numbers are replaced with ```0``` in the output when a source location is printed - useful if you want the same output from the testing framework even when test positions change within a source file |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| |

All the flags/options also come with a prefixed version (with ```--dt-``` at the front by default) - for example ```--version``` can be used also with ```--dt-version``` or ```--dt-v```.

The default prefix is ```--dt-```, but this can be changed by setting the [**```DOCTEST_CONFIG_OPTIONS_PREFIX```**](configuration.md#doctest_config_options_prefix) define.

All the unprefixed versions listed here can be disabled with the [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](configuration.md#doctest_config_no_unprefixed_options) define.

This is done for easy interoperability with client command line option handling when the testing framework is integrated within a client codebase - all **doctest** related flags/options can be prefixed so there are no clashes and so that the user can exclude everything starting with ```--dt-``` from their option parsing.

If there isn't an option to exclude those starting with ```--dt-``` then the ```dt_removed``` helper class might help to filter them out:

```c++
#define DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

class dt_removed {
    std::vector<const char*> vec;
public:
    dt_removed(const char** argv_in) {
        for(; *argv_in; ++argv_in)
            if(strncmp(*argv_in, "--dt-", strlen("--dt-")) != 0)
                vec.push_back(*argv_in);
        vec.push_back(NULL);
    }

    int          argc() { return static_cast<int>(vec.size()) - 1; }
    const char** argv() { return &vec[0]; } // Note: non-const char **:
};

int program(int argc, const char** argv);

int main(int argc, const char** argv) {
    doctest::Context context(argc, argv);
    int test_result = context.run(); // run queries, or run tests unless --no-run

    if(context.shouldExit()) // honor query flags and --exit
        return test_result;

    dt_removed args(argv);
    int app_result = program(args.argc(), args.argv());

    return test_result + app_result; // combine the 2 results
}

int program(int argc, const char** argv) {
    printf("Program: %d arguments received:\n", argc - 1);
    while(*++argv)
        printf("'%s'\n", *argv);
    return EXIT_SUCCESS;
}
```

When ran like this:

```
program.exe --dt-test-case=math* --my-option -s --dt-no-breaks
```

Will output this:

```
Program: 2 arguments received:
'--my-option'
'-s'
```

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
