#ifndef DOCTEST_MPI_H
#define DOCTEST_MPI_H

#ifdef DOCTEST_CONFIG_IMPLEMENT

#include "doctest/extensions/mpi_sub_comm.h"
#include "mpi_reporter.h"
#include <unordered_map>

namespace doctest {

// Each time a MPI_TEST_CASE is executed on N procs,
// we need a sub-communicator of N procs to execute it.
// It is then registered here and can be re-used
// by other tests that requires a sub-comm of the same size
std::unordered_map<int,mpi_sub_comm> sub_comms_by_size;

// Record if at least one MPI_TEST_CASE was registered "skipped"
// because there is not enought procs to execute it
int nb_test_cases_skipped_insufficient_procs = 0;


std::string thread_level_to_string(int thread_lvl);
int mpi_init_thread(int argc, char *argv[], int required_thread_support);
void mpi_finalize();


// Can be safely called before MPI_Init()
//   This is needed for MPI_TEST_CASE because we use doctest::skip()
//   to prevent execution of tests where there is not enough procs,
//   but doctest::skip() is called during test registration, that is, before main(), and hence before MPI_Init()
int mpi_comm_world_size() {
  #if defined(OPEN_MPI)
    const char* size_str = std::getenv("OMPI_COMM_WORLD_SIZE");
  #elif defined(I_MPI_VERSION) || defined(MPI_VERSION) // Intel MPI + MPICH (at least)
    const char* size_str = std::getenv("PMI_SIZE"); // see https://community.intel.com/t5/Intel-oneAPI-HPC-Toolkit/Environment-variables-defined-by-intel-mpirun/td-p/1096703
  #else
    #error "Unknown MPI implementation: please submit an issue or a PR to doctest. Meanwhile, you can look at the output of e.g. `mpirun -np 3 env` to search for an environnement variable that contains the size of MPI_COMM_WORLD and extend this code accordingly"
  #endif
  if (size_str==nullptr) return 1; // not launched with mpirun/mpiexec, so assume only one process
  return std::stoi(size_str);
}

// Record size of MPI_COMM_WORLD with mpi_comm_world_size()
int world_size_before_init = mpi_comm_world_size();


std::string thread_level_to_string(int thread_lvl) {
  switch (thread_lvl) {
    case MPI_THREAD_SINGLE:     return "MPI_THREAD_SINGLE";
    case MPI_THREAD_FUNNELED:   return "MPI_THREAD_FUNNELED";
    case MPI_THREAD_SERIALIZED: return "MPI_THREAD_SERIALIZED";
    case MPI_THREAD_MULTIPLE:   return "MPI_THREAD_MULTIPLE";
    default: return "Invalid MPI thread level";
  }
}
int mpi_init_thread(int argc, char *argv[], int required_thread_support) {
  int provided_thread_support;
  MPI_Init_thread(&argc, &argv, required_thread_support, &provided_thread_support);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  if (world_size_before_init != world_size) {
    DOCTEST_INTERNAL_ERROR(
      "doctest found "+std::to_string(world_size_before_init)+" MPI processes before `MPI_Init_thread`,"
      " but MPI_COMM_WORLD is actually of size "+std::to_string(world_size)+".\n"
      "This is most likely due to your MPI implementation not being well supported by doctest. Please report this issue on GitHub"
    );
  }

  if (provided_thread_support!=required_thread_support) {
    std::cout <<
        "WARNING: " + thread_level_to_string(required_thread_support) + " was asked, "
      + "but only " + thread_level_to_string(provided_thread_support) + " is provided by the MPI library\n";
  }
  return provided_thread_support;
}
void mpi_finalize() {
  // We need to destroy all created sub-communicators before calling MPI_Finalize()
  doctest::sub_comms_by_size.clear();
  MPI_Finalize();
}

} // doctest

#else // DOCTEST_CONFIG_IMPLEMENT

#include "doctest/extensions/mpi_sub_comm.h"
#include <unordered_map>
#include <exception>

namespace doctest {

extern std::unordered_map<int,mpi_sub_comm> sub_comms_by_size;
extern int nb_test_cases_skipped_insufficient_procs;
extern int world_size_before_init;
int mpi_comm_world_size();

int mpi_init_thread(int argc, char *argv[], int required_thread_support);
void mpi_finalize();

template<int nb_procs, class F>
void execute_mpi_test_case(F func) {
  auto it = sub_comms_by_size.find(nb_procs);
  if (it==end(sub_comms_by_size)) {
    bool was_emplaced = false;
    std::tie(it,was_emplaced) = sub_comms_by_size.emplace(std::make_pair(nb_procs,mpi_sub_comm(nb_procs)));
    assert(was_emplaced);
  }
  const mpi_sub_comm& sub = it->second;
  if (sub.comm != MPI_COMM_NULL) {
    func(sub.rank,nb_procs,sub.comm,std::integral_constant<int,nb_procs>{});
  };
}

inline bool
insufficient_procs(int test_nb_procs) {
  static const int world_size = mpi_comm_world_size();
  bool insufficient = test_nb_procs>world_size;
  if (insufficient) {
    ++nb_test_cases_skipped_insufficient_procs;
  }
  return insufficient;
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
  TEST_CASE(name * doctest::description("MPI_TEST_CASE") * doctest::skip(doctest::insufficient_procs(nb_procs))) { \
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

#endif // DOCTEST_MPI_H
