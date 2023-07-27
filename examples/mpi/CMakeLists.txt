find_package(MPI COMPONENTS CXX)
if(MPI_FOUND)
    add_executable(test_mpi main.cpp mpi.cpp)
    target_link_libraries(test_mpi doctest ${CMAKE_THREAD_LIBS_INIT} MPI::MPI_CXX)

    doctest_add_test(NO_OUTPUT NAME test_mpi COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 3 $<TARGET_FILE:test_mpi>)
endif()
