## Command line

**doctest** works quite nicely without any command line options at all - but for greater control use the following flags/options:


All the flags/options also come with a prefixed version (with ```-dt-``` at the front) - for example ```-rand-seed=<int>``` can be used also with ```-dt-rand-seed=<int>``` or ```-dt-rs=<int>```.

Also all the unprefixed versions listed here can be disabled with the [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](configuration.md) define.

This is done for easy interoperability with client command line option handling when the testing framework is integrated with a client codebase - all **doctest** related flags/options can be prefixed so there are no clashes and so that the user can exclude everything starting with ```-dt-``` from their option parsing.

Query flags - after the result is printed the program quits without executing any test cases (and if the framework is integrated into a client codebase and supplying it's own ```main()``` entry point - the program should check the result of ```doctest::Context::shouldExit()``` after calling ```doctest::Context::run()``` and should exit - this is left up to the user).

<table><tr><td>
Query Flags
</td><td>
Description
</td></tr><tr><td>
```-?,   --help, -h```
</td><td>
Prints a help message listing all these flags/options
</td></tr><tr><td>
```-v,   --version```
</td><td>
Prints the version of the **doctest** framework
</td></tr><tr><td>
```-c,   --count```
</td><td>
Prints the number of test cases matching the current filters (see below)
</td></tr><tr><td>
```-ltc, --list-test-cases```
</td><td>
Lists all test cases by name which match the current filters (see below)
</td></tr><tr><td>
```-lts, --list-test-suites```
</td><td>
Lists all test suites by name which have at least one test case matching the current filters (see below)
</td></tr></table>

```int```/```string``` options - they require a value after the ```=``` sign - without spaces! For example: ```--order-by=rand```.

Filters use wildcards for matching values - where ```*``` means "match any sequence" and ```?``` means "match any one character".

<table><tr><td>
Int/String Options
</td><td>
Description
</td></tr><tr><td>
```-tc,  --test-case=<filters>```
</td><td>
Filters test cases based on their name. By default all test cases match but if a value is given to this filter like ```--test-case=*math*,*sound*``` then only test cases who match atleast one of the patterns in the comma-separated list with wildcards will get executed/counted/listed.
</td></tr><tr><td>
```-tce, --test-case-exclude=<filters>```
</td><td>
same as the ```-test-case=<filters>``` option but if any of the patterns in the comma-separated list of values matches - then the test case is skipped.
</td></tr><tr><td>
```-sf,  --source-file=<filters>```
</td><td>
filters     tests by their file
</td></tr><tr><td>
```-sfe, --source-file-exclude=<filters>```
</td><td>
filters OUT tests by their file
</td></tr><tr><td>
```-ts,  --test-suite=<filters>```
</td><td>
filters     tests by their test suite
</td></tr><tr><td>
```-tse, --test-suite-exclude=<filters>```
</td><td>
filters OUT tests by their test suite
</td></tr><tr><td>
```-ob,  --order-by=<string>```
</td><td>
how the tests should be ordered <string> - by [file/suite/name/rand]
</td></tr><tr><td>
```-rs,  --rand-seed=<int>```
</td><td>
seed for random ordering
</td></tr><tr><td>
```-f,   --first=<int>```
</td><td>
the first test passing the filters to execute - for range-based execution
</td></tr><tr><td>
```-l,   --last=<int>```
</td><td>
the last test passing the filters to execute - for range-based execution
</td></tr><tr><td>
```-aa,  --abort-after=<int>```
</td><td>
stop after <int> failed assertions
</td></tr></table>

<table><tr><td>
Bool Options
</td><td>
Description
</td></tr><tr><td>
```-s,   --success=<bool>```
</td><td>
include successful assertions in output
</td></tr><tr><td>
```-cs,  --case-sensitive=<bool>```
</td><td>
filters being treated as case sensitive
</td></tr><tr><td>
```-e,   --exit=<bool>```
</td><td>
exits after the tests finish
</td></tr><tr><td>
```-no,  --no-overrides=<bool>```
</td><td>
disables procedural overrides of options
</td></tr><tr><td>
```-nt,  --no-throw=<bool>```
</td><td>
skips exceptions-related assert checks
</td></tr><tr><td>
```-ne,  --no-exitcode=<bool>```
</td><td>
returns (or exits) always with success
</td></tr><tr><td>
```-nr,  --no-run=<bool>```
</td><td>
skips all runtime doctest operations
</td></tr><tr><td>
```-nc,  --no-colors=<bool>```
</td><td>
disables colors in output
</td></tr><tr><td>
```-nb,  --no-breaks=<bool>```
</td><td>
disables breakpoints in debuggers
</td></tr><tr><td>
```-npf, --no-path-filenames=<bool>```
</td><td>
only filenames and no paths in output
</td></tr></table>
