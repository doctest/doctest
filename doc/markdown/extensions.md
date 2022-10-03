## Extensions

The doctest header doesn't include any external or stdlib headers in its interface part in order to provide the most optimal build times but that means it is limited in what it can provide as functionality => that's when extensions come into play. They are located as header files in [`doctest/extensions`](../../doctest/extensions) and each of them is documented in a section here.

# [Utils](../../doctest/extensions/doctest_util.h)

nothing here yet...

# [Distributed tests with MPI](../../doctest/extensions/doctest_mpi.h)

[Bruno Maugars and Bérenger Berthoul, ONERA]

Testing code over distributed processes requires support from the testing framework. **Doctest** support for MPI parallel communication is provided in the ```"doctest/extensions/doctest_mpi.h"``` header.

## Example

See [**the complete test**](../../examples/mpi/mpi.cpp) and [**the configuration of main()**](../../examples/mpi/main.cpp)

### MPI_TEST_CASE

```c++
#include "doctest/extensions/doctest_mpi.h"

int my_function_to_test(MPI_Comm comm) {
  int rank;
  MPI_Comm_rank(comm,&rank);
  if (rank == 0) {
    return 10;
  }
  return 11;
}


MPI_TEST_CASE("test over two processes",2) { // Parallel test on 2 processes
  int x = my_function_to_test(test_comm);

  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
}
```

An ```MPI_TEST_CASE``` is like a regular ```TEST_CASE```, except it takes a second argument, which is the number of processes needed to run the test.  If the number of processes is less than 2, the test will fail. If the number of processes is greater than or equal to 2, it will create a sub-communicator over 2 processes, called ```test_comm```, and execute the test over these processes. Three objects are provided by ```MPI_TEST_CASE```:
 * ```test_comm```, of type ```MPI_Comm```: the mpi communicator on which the test is running,
 * ```test_rank``` and ```test_nb_procs```, two ```int``` giving respectively the rank of the current process and the size of the communicator for ```test_comm```. These last two are just here for convenience and could be retrieved from ```test_comm```.

We always have:

```c++
MPI_TEST_CASE("my_test",N) {
  CHECK( test_nb_procs == N );
  MPI_CHECK( i, test_rank==i ); // for any i<N
}
```

### Assertions
It is possible to use regular assertions in an ```MPI_TEST_CASE```. MPI-specific assertions are also provided and are all prefixed with ```MPI_``` (```MPI_CHECK```, ```MPI_ASSERT```...). The first argument is the rank for which they are checked, and the second is the usual expression to check.

## The main entry points and mpi reporters

You need to launch the unit tests with an ```mpirun``` or ```mpiexec``` command:
```
mpirun -np 2 unit_test_executable.exe
```

```doctest::mpi_init_thread()``` must be called before running the unit tests, and ```doctest::mpi_finalize()``` at the end of the program. Also, using the default console reporter will result in each process writing everything in the same place, which is not what we want. Two reporters are provided and can be enabled. A complete ```main()``` would be:


```c++
#define DOCTEST_CONFIG_IMPLEMENT

#include "doctest/extensions/doctest_mpi.h"

int main(int argc, char** argv) {
  doctest::mpi_init_thread(argc,argv,MPI_THREAD_MULTIPLE); // Or any MPI thread level

  doctest::Context ctx;
  ctx.setOption("reporters", "MpiConsoleReporter");
  ctx.setOption("reporters", "MpiFileReporter");
  ctx.setOption("force-colors", true);
  ctx.applyCommandLine(argc, argv);

  int test_result = ctx.run();

  doctest::mpi_finalize();

  return test_result;
}
```

### MpiConsoleReporter

The ```MpiConsoleReporter``` should be substituted to the default reporter. It does the same as the default console reporter for regular assertions, but only outputs on process 0. For MPI test cases, if there is a failure it tells the process that failed

```
[doctest] doctest version is "2.4.0"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:    171 |    171 passed |      0 failed |      0 skipped
[doctest] assertions:    864 |    864 passed |      0 failed |
[doctest] Status: SUCCESS!
std_e_mpi_unit_tests
[doctest] doctest version is "2.4.0"
[doctest] run with "--help" for options
===============================================================================
path/to/test.cpp:30:
TEST CASE: my test case

On rank [2] : path/to/test.cpp:35: CHECK( x==-1 ) is NOT correct!
  values: CHECK( 0 == -1 )

===============================================================================
[doctest] test cases:      2 |      2 passed |      0 failed |      0 skipped
[doctest] assertions:      2 |      2 passed |      0 failed |
[doctest] Status: SUCCESS!
===============================================================================
[doctest] assertions on all processes:   5 |   4 passed |      1 failed |
===============================================================================
[doctest] fail on rank:
    -> On rank [2] with 1 test failed
[doctest] Status: FAILURE!
```

If the test executable is launch with less processes than the number of processes required by one test, the test is skipped and marqued as such in the mpi console reporter:


```c++
MPI_TEST_CASE("my_test",3) {
  // ...
}
```

```
mpirun -np 2 unit_test_executable.exe
```

```
===============================================================================
[doctest] test cases:      1 |      1 passed |      0 failed |      1 skipped
[doctest] assertions:      1 |      1 passed |      0 failed |
[doctest] Status: SUCCESS!
===============================================================================
[doctest] assertions on all processes:   1 |   1 passed |      0 failed |
[doctest] WARNING: Skipped 1 test requiring more than 2 MPI processes to run
===============================================================================
```

### MpiFileReporter
The ```MpiFileReporter``` will just print the result of each process in its own file, named ```doctest_[rank].log```. Only use this reporter as a debug facility if you want to know what is going on exactly when a parallel test case is failing.

### Other reporters
Other reporters (jUnit, XML) are not supported directly, which mean that you can always print the result of each process to its own file, but there is (currently) no equivalent of the ```MpiConsoleReporter``` that will aggregate the results of all processes.


## Note

This feature is provided to unit-test mpi-distributed code. It is **not** a way to parallelize many unit tests over several processes (for that, see [**the example python script**](../../examples/range_based_execution.py)).

## TODO

 * Pass ```s``` member variable of ```ConsoleReporter``` as an argument to member functions so we can use them with another object (would help to factorize ```MPIConsoleReporter```)
 * Only MPI_CHECK tested. MPI_REQUIRE, exception handling: nothing tested
 * More testing, automatic testing
 * Packaging: create a new target ```mpi_doctest```? (probably cleaner to depend explicitly on MPI for mpi/doctest.h)
 * Later, maybe: have a general mechanism to represent assertions so we can separate the report format (console, xml, junit...) from the reporting strategy (sequential vs. MPI)

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
