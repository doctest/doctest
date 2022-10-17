#ifndef DOCTEST_MPI_SUB_COMM_H
#define DOCTEST_MPI_SUB_COMM_H

#include "mpi.h"
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

  void destroy_comm() {
    if(comm != MPI_COMM_NULL){
      MPI_Comm_free(&comm);
    }
  }
  
  mpi_sub_comm(mpi_sub_comm&& x)
    : nb_procs(x.nb_procs)
    , rank(x.rank)
    , comm(x.comm)
  {
    x.comm = MPI_COMM_NULL;
  }
  mpi_sub_comm& operator=(mpi_sub_comm&& x) {
    destroy_comm();
    nb_procs = x.nb_procs;
    rank = x.rank;
    comm = x.comm;
    x.comm = MPI_COMM_NULL;
    return *this;
  }

  ~mpi_sub_comm() {
    destroy_comm();
  }
};

} // doctest

#endif // DOCTEST_SUB_COMM_H
