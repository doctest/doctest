#pragma once

#ifdef DOCTEST_CONFIG_IMPLEMENT

#include "doctest/doctest.h"
#include "mpi_reporter.h"

#else

#include "mpi.h"
#include <numeric>
#include <vector>
#include "doctest/doctest.h"
#include <cassert>
#include <string>

namespace doctest {

inline
int mpi_world_nb_procs() {
  int n;
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  return n;
}

struct mpi_sub_comm {
  int nb_procs;
  int rank;
  MPI_Comm comm;

  mpi_sub_comm( mpi_sub_comm const& ) = delete;
  mpi_sub_comm& operator=( mpi_sub_comm const& ) = delete;

  mpi_sub_comm(int nb_prcs) noexcept
    : nb_procs(nb_prcs)
    , rank(-1)
    , comm(MPI_COMM_NULL)
  {
    int comm_world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_world_rank);
    if (nb_procs>mpi_world_nb_procs()) {
      if (comm_world_rank==0) {
        MESSAGE(
          "Unable to run test: need ", std::to_string(nb_procs), " procs", 
          " but program launched with only ", std::to_string(doctest::mpi_world_nb_procs()), "."
        );
        CHECK(nb_procs<=mpi_world_nb_procs());
      }
    } else {
      int color = MPI_UNDEFINED;
      if(comm_world_rank < nb_procs){
        color = 0;
      }
      MPI_Comm_split(MPI_COMM_WORLD, color, comm_world_rank, &comm);

      if(comm != MPI_COMM_NULL){
        MPI_Comm_rank(comm, &rank);
        assert(rank==comm_world_rank);
      }
    }
  }

  ~mpi_sub_comm() {
    if(comm != MPI_COMM_NULL){
      MPI_Comm_free(&comm);
    }
  }
};


template<int nb_procs, class F>
void execute_mpi_test_case(F func) {
  mpi_sub_comm sub(nb_procs);
  if (sub.comm != MPI_COMM_NULL) {
    func(sub.rank,nb_procs,sub.comm,std::integral_constant<int,nb_procs>{});
  };
}

} // doctest


#define DOCTEST_MPI_GEN_ASSERTION(rank_to_test, assertion, ...) \
  static_assert(rank_to_test<test_nb_procs_as_int_constant.value,"Trying to assert on a rank greater than the number of procs of the test!"); \
  if(rank_to_test == test_rank) assertion(__VA_ARGS__)

#define DOCTEST_MPI_WARN(rank_to_test, ...)  DOCTEST_MPI_GEN_ASSERTION(rank_to_test,DOCTEST_WARN,__VA_ARGS__)
#define DOCTEST_MPI_CHECK(rank_to_test, ...)  DOCTEST_MPI_GEN_ASSERTION(rank_to_test,DOCTEST_CHECK,__VA_ARGS__)
#define DOCTEST_MPI_REQUIRE(rank_to_test, ...)  DOCTEST_MPI_GEN_ASSERTION(rank_to_test,DOCTEST_REQUIRE,__VA_ARGS__)
#define DOCTEST_MPI_WARN_FALSE(rank_to_test, ...)  DOCTEST_MPI_GEN_ASSERTION(rank_to_test,DOCTEST_WARN_FALSE,__VA_ARGS__)
#define DOCTEST_MPI_CHECK_FALSE(rank_to_test, ...)  DOCTEST_MPI_GEN_ASSERTION(rank_to_test,DOCTEST_CHECK_FALSE,__VA_ARGS__)
#define DOCTEST_MPI_REQUIRE_FALSE(rank_to_test, ...)  DOCTEST_MPI_GEN_ASSERTION(rank_to_test,DOCTEST_REQUIRE_FALSE,__VA_ARGS__)

#define DOCTEST_CREATE_MPI_TEST_CASE(name,nb_procs,func) \
  static void func(DOCTEST_UNUSED int test_rank, DOCTEST_UNUSED int test_nb_procs, DOCTEST_UNUSED MPI_Comm test_comm, DOCTEST_UNUSED std::integral_constant<int,nb_procs>); \
  TEST_CASE(name * doctest::description("MPI_TEST_CASE")) { \
    doctest::execute_mpi_test_case<nb_procs>(func); \
  } \
  static void func(DOCTEST_UNUSED int test_rank, DOCTEST_UNUSED int test_nb_procs, DOCTEST_UNUSED MPI_Comm test_comm, DOCTEST_UNUSED std::integral_constant<int,nb_procs> test_nb_procs_as_int_constant)
  // DOC: test_rank, test_nb_procs, and test_comm are available UNDER THESE SPECIFIC NAMES in the body of the unit test
  // DOC: test_nb_procs_as_int_constant is equal to test_nb_procs, but as a compile time value
  //          (used in CHECK-like macros to assert the checked rank exists)

#define DOCTEST_MPI_TEST_CASE(name,nb_procs) \
  DOCTEST_CREATE_MPI_TEST_CASE(name,nb_procs,DOCTEST_ANONYMOUS(DOCTEST_MPI_FUNC))


// == SHORT VERSIONS OF THE MACROS
#if !defined(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)
#define MPI_WARN           DOCTEST_MPI_WARN
#define MPI_CHECK          DOCTEST_MPI_CHECK
#define MPI_REQUIRE        DOCTEST_MPI_REQUIRE
#define MPI_WARN_FALSE     DOCTEST_MPI_WARN_FALSE
#define MPI_CHECK_FALSE    DOCTEST_MPI_CHECK_FALSE
#define MPI_REQUIRE_FALSE  DOCTEST_MPI_REQUIRE_FALSE

#define MPI_TEST_CASE      DOCTEST_MPI_TEST_CASE
#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES


#endif // DOCTEST_CONFIG_IMPLEMENT
