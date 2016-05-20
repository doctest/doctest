## Command line

**doctest** works quite nicely without any command line options at all - but for more control there are a bunch that are available.

All the flags/options also come with a prefixed version (with ```-dt-``` at the front) - for example ```-rand-seed=(int)``` can be used also with ```-dt-rand-seed=(int)``` or ```-dt-rs=(int)```.

Also all the unprefixed versions listed here can be disabled with the [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](configuration.md) define.

This is done for easy interoperability with client command line option handling when the testing framework is integrated with a client codebase - all **doctest** related flags/options can be prefixed so there are no clashes and so that the user can exclude everything starting with ```-dt-``` from their option parsing.

**Query flags** - after the result is printed the program quits without executing any test cases (and if the framework is integrated into a client codebase and [**supplying it's own ```main()``` entry point**](main.md) - the program should check the result of ```doctest::Context::shouldExit()``` after calling ```doctest::Context::run()``` and should exit - this is left up to the user).

```int```/```string``` options - they require a value after the ```=``` sign - without spaces! For example: ```--order-by=rand```.

Filters use wildcards for matching values - where ```*``` means "match any sequence" and ```?``` means "match any one character".

| **Query Flags**                                  | **Description**                                                                                                                                                                                                                                                                              |
|--------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ```-?```  ```--help``` ```-h```                  | Prints a help message listing all these flags/options                                                                                                                                                                                                                                        |
| ```-v```   ```--version```                       | Prints the version of the **doctest** framework                                                                                                                                                                                                                                              |
| ```-c```   ```--count```                         | Prints the number of test cases matching the current filters (see below)                                                                                                                                                                                                                     |
| ```-ltc``` ```--list-test-cases```               | Lists all test cases by name which match the current filters (see below)                                                                                                                                                                                                                     |
| ```-lts``` ```--list-test-suites```              | Lists all test suites by name which have at least one test case matching the current filters (see below)                                                                                                                                                                                     |
| **Int/String Options**                           |                                                                                                                                                                                                                                                                                              |
| ```-tc``` ```--test-case=(filters)```            | Filters test cases based on their name. By default all test cases match but if a value is given to this filter like ```--test-case=*math*,*sound*``` then only test cases who match atleast one of the patterns in the comma-separated list with wildcards will get executed/counted/listed. |
| ```-tce``` ```--test-case-exclude=(filters)```   | same as the ```-test-case=(filters)``` option but if any of the patterns in the comma-separated list of values matches - then the test case is skipped.                                                                                                                                      |
| ```-sf``` ```--source-file=(filters)```          | filters tests by their file                                                                                                                                                                                                                                                                  |
| ```-sfe``` ```--source-file-exclude=(filters)``` | filters OUT tests by their file                                                                                                                                                                                                                                                              |
| ```-ts``` ```--test-suite=(filters)```           | filters tests by their test suite                                                                                                                                                                                                                                                            |
| ```-tse``` ```--test-suite-exclude=(filters)```  | filters OUT tests by their test suite                                                                                                                                                                                                                                                        |
| ```-ob``` ```--order-by=(string)```              | how the tests should be ordered (string) - by [file/suite/name/rand]                                                                                                                                                                                                                         |
| ```-rs``` ```--rand-seed=(int)```                | seed for random ordering                                                                                                                                                                                                                                                                     |
| ```-f``` ```--first=(int)```                     | the first test passing the filters to execute - for range-based execution                                                                                                                                                                                                                    |
| ```-l``` ```--last=(int)```                      | the last test passing the filters to execute - for range-based execution                                                                                                                                                                                                                     |
| ```-aa``` ```--abort-after=(int)```              | stop after (int) failed assertions                                                                                                                                                                                                                                                           |
| Bool Options                                     |                                                                                                                                                                                                                                                                                              |
| ```-s``` ```--success=(bool)```                  | include successful assertions in output                                                                                                                                                                                                                                                      |
| ```-cs``` ```--case-sensitive=(bool)```          | filters being treated as case sensitive                                                                                                                                                                                                                                                      |
| ```-e``` ```--exit=(bool)```                     | exits after the tests finish                                                                                                                                                                                                                                                                 |
| ```-no``` ```--no-overrides=(bool)```            | disables procedural overrides of options                                                                                                                                                                                                                                                     |
| ```-nt``` ```--no-throw=(bool)```                | skips exceptions-related assert checks                                                                                                                                                                                                                                                       |
| ```-ne``` ```--no-exitcode=(bool)```             | returns (or exits) always with success                                                                                                                                                                                                                                                       |
| ```-nr``` ```--no-run=(bool)```                  | skips all runtime doctest operations                                                                                                                                                                                                                                                         |
| ```-nc``` ```--no-colors=(bool)```               | disables colors in output                                                                                                                                                                                                                                                                    |
| ```-nb``` ```--no-breaks=(bool)```               | disables breakpoints in debuggers                                                                                                                                                                                                                                                            |
| ```-npf``` ```--no-path-filenames=(bool)```      | only filenames and no paths in output                                                                                                                                                                                                                                                        |

<table><tr><td>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<b>Query Flags</b>
</td><td>
<b>Description</b>
</td></tr><tr><td>
<pre>-?,   --help, -h</pre>
</td><td>
Prints a help message listing all these flags/options
</td></tr><tr><td>
<pre>-v,   --version</pre>
</td><td>
Prints the version of the <b>doctest</b> framework
</td></tr><tr><td>
<pre>-c,   --count</pre>
</td><td>
Prints the number of test cases matching the current filters (see below)
</td></tr><tr><td>
<pre>-ltc, --list-test-cases</pre>
</td><td>
Lists all test cases by name which match the current filters (see below)
</td></tr><tr><td>
<pre>-lts, --list-test-suites</pre>
</td><td>
Lists all test suites by name which have at least one test case matching the current filters (see below)
</td></tr></table>

<table><tr><td>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<b>Int/String Options</b>
</td><td>
<b>Description</b>
</td></tr><tr><td>
<pre>-tc,  --test-case=(filters)</pre>
</td><td>
Filters test cases based on their name. By default all test cases match but if a value is given to this filter like ```--test-case=*math*,*sound*``` then only test cases who match atleast one of the patterns in the comma-separated list with wildcards will get executed/counted/listed.
</td></tr><tr><td>
<pre>-tce, --test-case-exclude=(filters)</pre>
</td><td>
same as the ```-test-case=(filters)``` option but if any of the patterns in the comma-separated list of values matches - then the test case is skipped.
</td></tr><tr><td>
<pre>-sf,  --source-file=(filters)</pre>
</td><td>
filters     tests by their file
</td></tr><tr><td>
<pre>-sfe, --source-file-exclude=(filters)</pre>
</td><td>
filters OUT tests by their file
</td></tr><tr><td>
<pre>-ts,  --test-suite=(filters)</pre>
</td><td>
filters     tests by their test suite
</td></tr><tr><td>
<pre>-tse, --test-suite-exclude=(filters)</pre>
</td><td>
filters OUT tests by their test suite
</td></tr><tr><td>
<pre>-ob,  --order-by=(string)</pre>
</td><td>
how the tests should be ordered (string) - by [file/suite/name/rand]
</td></tr><tr><td>
<pre>-rs,  --rand-seed=(int)</pre>
</td><td>
seed for random ordering
</td></tr><tr><td>
<pre>-f,   --first=(int)</pre>
</td><td>
the first test passing the filters to execute - for range-based execution
</td></tr><tr><td>
<pre>-l,   --last=(int)</pre>
</td><td>
the last test passing the filters to execute - for range-based execution
</td></tr><tr><td>
<pre>-aa,  --abort-after=(int)</pre>
</td><td>
stop after (int) failed assertions
</td></tr></table>

<table><tr><td>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<b>Bool Options</b>
</td><td>
<b>Description</b>
</td></tr><tr><td>
<pre>-s,   --success=(bool)</pre>
</td><td>
include successful assertions in output
</td></tr><tr><td>
<pre>-cs,  --case-sensitive=(bool)</pre>
</td><td>
filters being treated as case sensitive
</td></tr><tr><td>
<pre>-e,   --exit=(bool)</pre>
</td><td>
exits after the tests finish
</td></tr><tr><td>
<pre>-no,  --no-overrides=(bool)</pre>
</td><td>
disables procedural overrides of options
</td></tr><tr><td>
<pre>-nt,  --no-throw=(bool)</pre>
</td><td>
skips exceptions-related assert checks
</td></tr><tr><td>
<pre>-ne,  --no-exitcode=(bool)</pre>
</td><td>
returns (or exits) always with success
</td></tr><tr><td>
<pre>-nr,  --no-run=(bool)</pre>
</td><td>
skips all runtime doctest operations
</td></tr><tr><td>
<pre>-nc,  --no-colors=(bool)</pre>
</td><td>
disables colors in output
</td></tr><tr><td>
<pre>-nb,  --no-breaks=(bool)</pre>
</td><td>
disables breakpoints in debuggers
</td></tr><tr><td>
<pre>-npf, --no-path-filenames=(bool)</pre>
</td><td>
only filenames and no paths in output
</td></tr></table>