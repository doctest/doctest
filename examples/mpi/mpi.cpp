#include <doctest/extensions/doctest_mpi.h>

int f_for_test(int rank);

int f_for_test(int rank) {
  if (rank == 0) {
    return 10;
  }
  else if (rank == 1) {
    return 11;
  }
  return 0;
}



MPI_TEST_CASE("Parallel test on 2 processes",2) { // if MPI_SIZE < 2, report test can't be run
  // 3 objects accessible in the test:
  //    test_comm: MPI_Comm of size 2
  //    test_rank: integer of value the rank of the process in test_comm
  //    test_nb_procs: integer of value the size of the process (here: 2)

  int x = f_for_test(test_rank);

  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
  //MPI_CHECK( 2,  x==0 ); // will trigger a static assert because non-existing rank
}

MPI_TEST_CASE("Parallel test on 3 processes (failing)",3) {
  int x = f_for_test(test_rank);

  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
  MPI_CHECK( 2,  x==-1 ); // CHECK for rank 2, that x==-1 (which is not the case -> will trigger a failure report)
}

MPI_TEST_CASE("Parallel tests with subcases",2) {

  SUBCASE("sub_case 0") {
    CHECK( test_nb_procs == 2 );
  }
  SUBCASE("sub_case 1") {
    CHECK( test_rank == 0 ); // should fail on proc 1
  }
}
