## Command line

**doctest** works quite nicely without any command line options at all - but for more control there are a bunch that are available.

**Query flags** - after the result is printed the program quits without executing any test cases (and if the framework is integrated into a client codebase which [**supplies it's own ```main()``` entry point**](main.md) - the program should check the result of ```doctest::Context::shouldExit()``` after calling ```doctest::Context::run()``` and should exit - this is left up to the user).

**Int/String options** - they require a value after the ```=``` sign - without spaces! For example: ```--order-by=rand```.

**Bool options** - they expect ```1```/```yes```/```on```/```true``` or ```0```/```no```/```off```/```false``` after the ```=``` sign - but they can also be used like flags and the ```=value``` part can be skipped - then ```true``` is assumed.  

**Filters** use wildcards for matching values - where ```*``` means "match any sequence" and ```?``` means "match any one character".
To pass a pattern with an interval use ```""``` like this:  ```--test-case="*no sound*,vaguely named test number ?"```.

All the options can also be set with code if the user [**supplies the ```main()``` function**](main.md) so an option is always with some value.

| Query Flags | Description |
|:------------|-------------|
| ```-?``` &nbsp;&nbsp;&nbsp; ```--help``` ```-h``` | Prints a help message listing all these flags/options |
| ```-v``` &nbsp;&nbsp;&nbsp; ```--version``` | Prints the version of the **doctest** framework |
| ```-c``` &nbsp;&nbsp;&nbsp; ```--count``` | Prints the number of test cases matching the current filters (see below) |
| ```-ltc``` ```--list-test-cases``` | Lists all test cases by name which match the current filters (see below) |
| ```-lts``` ```--list-test-suites``` | Lists all test suites by name which have at least one test case matching the current filters (see below) |
| **Int/String Options** | <hr> |
| ```-tc``` &nbsp; ```--test-case=<filters>``` | Filters test cases based on their name. By default all test cases match but if a value is given to this filter like ```--test-case=*math*,*sound*``` then only test cases who match atleast one of the patterns in the comma-separated list with wildcards will get executed/counted/listed |
| ```-tce``` ```--test-case-exclude=<filters>``` | Same as the ```-test-case=<filters>``` option but if any of the patterns in the comma-separated list of values matches - then the test case is skipped |
| ```-sf``` &nbsp; ```--source-file=<filters>``` | Same as ```--test-case=<filters>``` but filters based on the file in which test cases are written |
| ```-sfe``` ```--source-file-exclude=<filters>``` | Same as ```--test-case-exclude=<filters>``` but filters based on the file in which test cases are written |
| ```-ts``` &nbsp; ```--test-suite=<filters>``` | Same as ```--test-case=<filters>``` but filters based on the test suite in which test cases are in |
| ```-tse``` ```--test-suite-exclude=<filters>``` | Same as ```--test-case-exclude=<filters>``` but filters based on the test suite in which test cases are in |
| ```-ob``` &nbsp; ```--order-by=<string>``` | Test cases will be sorted before being executed either by **the file in which they are** / **the test suite they are in** / **their name** / **random**. The possible values of ```<string>``` are ```file```/```suite```/```name```/```rand```. The default is ```file``` |
| ```-rs``` &nbsp; ```--rand-seed=<int>``` | The seed for random ordering |
| ```-f``` &nbsp;&nbsp;&nbsp; ```--first=<int>``` | The first test case to execute which passes the current filters - for range-based execution - see [**the multiprocess example**](../../examples/multiprocess/) (the **run.py** script) |
| ```-l``` &nbsp;&nbsp;&nbsp; ```--last=<int>``` | The last test case to execute which passes the current filters - for range-based execution - see [**the multiprocess example**](../../examples/multiprocess/) (the **run.py** script) |
| ```-aa``` &nbsp; ```--abort-after=<int>``` | The testing framework will stop executing test cases/assertions after this many failed assertions. The default is 0 which means don't stop at all |
| **Bool Options** | <hr> |
| ```-s``` &nbsp;&nbsp;&nbsp; ```--success=<bool>``` | To include successful assertions in the output |
| ```-cs``` &nbsp; ```--case-sensitive=<bool>``` | Filters being treated as case sensitive |
| ```-e``` &nbsp;&nbsp;&nbsp; ```--exit=<bool>``` | Exits after the tests finish - this is meaningful only when the client has [**provided the ```main()``` entry point**](main.md)  - the program should check ```doctest::Context::shouldExit()``` after calling ```doctest::Context::run()``` and should exit - this is left up to the user. The idea is to be able to execute just the tests in a client program and to not continue with it's execution |
| ```-no``` &nbsp; ```--no-overrides=<bool>``` | Disables procedural overrides of options which are only possible if the client has [**provided the ```main()``` entry point**](main.md). This is useful if the program has some default options set (which override the command line) but you want to set an option differently from the command line without recompiling.|
| ```-nt``` &nbsp; ```--no-throw=<bool>``` | Skips [**exceptions-related assertion**](assertions.md) checks |
| ```-ne``` &nbsp; ```--no-exitcode=<bool>``` | Always returns a successful exit code - even if a test case has failed |
| ```-nr``` &nbsp; ```--no-run=<bool>``` | Skips all runtime **doctest** operations (except the test registering which happens before the program enters ```main()```). This is useful if the testing framework is integrated into a client codebase which has [**provided the ```main()``` entry point**](main.md) and the user wants to skip running the tests and just use the program |
| ```-nc``` &nbsp; ```--no-colors=<bool>``` | Disables colors in the output |
| ```-nb``` &nbsp; ```--no-breaks=<bool>``` | Disables breakpoints in debuggers when an assertion fails |
| ```-npf``` ```--no-path-filenames=<bool>``` | Paths are removed from the output when a filename is printed - useful if you want the same output from the testing framework on different environments |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| |

All the flags/options also come with a prefixed version (with ```-dt-``` at the front) - for example ```--version``` can be used also with ```--dt-version``` or ```-dt-v```.

All the unprefixed versions listed here can be disabled with the [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](configuration.md) define.

This is done for easy interoperability with client command line option handling when the testing framework is integrated within a client codebase - all **doctest** related flags/options can be prefixed so there are no clashes and so that the user can exclude everything starting with ```-dt-``` from their option parsing.

---------------

[Home](readme.md#reference)
