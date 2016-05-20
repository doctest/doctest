## Command line

**doctest** works quite nicely without any command line options at all - but for more control there are a bunch that are available.

All the flags/options also come with a prefixed version (with ```-dt-``` at the front) - for example ```-rand-seed=(int)``` can be used also with ```-dt-rand-seed=(int)``` or ```-dt-rs=(int)```.

Also all the unprefixed versions listed here can be disabled with the [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](configuration.md) define.

This is done for easy interoperability with client command line option handling when the testing framework is integrated with a client codebase - all **doctest** related flags/options can be prefixed so there are no clashes and so that the user can exclude everything starting with ```-dt-``` from their option parsing.

**Query flags** - after the result is printed the program quits without executing any test cases (and if the framework is integrated into a client codebase and [**supplying it's own ```main()``` entry point**](main.md) - the program should check the result of ```doctest::Context::shouldExit()``` after calling ```doctest::Context::run()``` and should exit - this is left up to the user).

```int```/```string``` options - they require a value after the ```=``` sign - without spaces! For example: ```--order-by=rand```.

Filters use wildcards for matching values - where ```*``` means "match any sequence" and ```?``` means "match any one character".



| Query Flags | Description |
|:-------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ```-?``` &nbsp;&nbsp;&nbsp; ```--help``` ```-h``` | Prints a help message listing all these flags/options |
| ```-v``` &nbsp;&nbsp;&nbsp; ```--version``` | Prints the version of the **doctest** framework |
| ```-c``` &nbsp;&nbsp;&nbsp; ```--count``` | Prints the number of test cases matching the current filters (see below) |
| ```-ltc``` ```--list-test-cases``` | Lists all test cases by name which match the current filters (see below) |
| ```-lts``` ```--list-test-suites``` | Lists all test suites by name which have at least one test case matching the current filters (see below) |
| **Int/String Options** | <hr> |
| ```-tc``` &nbsp; ```--test-case=(filters)``` | Filters test cases based on their name. By default all test cases match but if a value is given to this filter like ```--test-case=*math*,*sound*``` then only test cases who match atleast one of the patterns in the comma-separated list with wildcards will get executed/counted/listed. |
| ```-tce``` ```--test-case-exclude=(filters)``` | same as the ```-test-case=(filters)``` option but if any of the patterns in the comma-separated list of values matches - then the test case is skipped. |
| ```-sf``` &nbsp; ```--source-file=(filters)``` | filters tests by their file |
| ```-sfe``` ```--source-file-exclude=(filters)``` | filters OUT tests by their file |
| ```-ts``` &nbsp; ```--test-suite=(filters)``` | filters tests by their test suite |
| ```-tse``` ```--test-suite-exclude=(filters)``` | filters OUT tests by their test suite |
| ```-ob``` &nbsp; ```--order-by=(string)``` | how the tests should be ordered (string) - by [file/suite/name/rand] |
| ```-rs``` &nbsp; ```--rand-seed=(int)``` | seed for random ordering |
| ```-f``` &nbsp;&nbsp;&nbsp; ```--first=(int)``` | the first test passing the filters to execute - for range-based execution |
| ```-l``` &nbsp;&nbsp;&nbsp; ```--last=(int)``` | the last test passing the filters to execute - for range-based execution |
| ```-aa``` &nbsp; ```--abort-after=(int)``` | stop after (int) failed assertions |
| **Bool Options** | <hr> |
| ```-s``` &nbsp;&nbsp;&nbsp; ```--success=(bool)``` | include successful assertions in output |
| ```-cs``` &nbsp; ```--case-sensitive=(bool)``` | filters being treated as case sensitive |
| ```-e``` &nbsp;&nbsp;&nbsp; ```--exit=(bool)``` | exits after the tests finish |
| ```-no``` &nbsp; ```--no-overrides=(bool)``` | disables procedural overrides of options |
| ```-nt``` &nbsp; ```--no-throw=(bool)``` | skips exceptions-related assert checks |
| ```-ne``` &nbsp; ```--no-exitcode=(bool)``` | returns (or exits) always with success |
| ```-nr``` &nbsp; ```--no-run=(bool)``` | skips all runtime doctest operations |
| ```-nc``` &nbsp; ```--no-colors=(bool)``` | disables colors in output |
| ```-nb``` &nbsp; ```--no-breaks=(bool)``` | disables breakpoints in debuggers |
| ```-npf``` ```--no-path-filenames=(bool)``` | only filenames and no paths in output |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| |
