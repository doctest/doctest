################################################################################
## BUILD ALL EXAMPLE SOURCES INTO A SINGLE BINARY AND EXECUTE TESTS ON EACH FILE
################################################################################

# TODO factor with example/all_features/CMakeLists.txt

set(files_with_output
    main.cpp
    mpi.cpp
)

set(files_all
    ${files_with_output}
)

find_package(MPI COMPONENTS CXX)
if(MPI_FOUND)
  # add the executable
  add_executable(mpi_features ${files_all})
  target_link_libraries(mpi_features doctest ${CMAKE_THREAD_LIBS_INIT} MPI::MPI_CXX)

  # easy way to fix test coverage - disable colors and crash handling
  target_compile_definitions(mpi_features PRIVATE
      DOCTEST_CONFIG_COLORS_NONE
      DOCTEST_CONFIG_NO_POSIX_SIGNALS
      DOCTEST_CONFIG_NO_WINDOWS_SEH)

  # omit the version and the num test cases skipped from the summary - this way the output will change less often
  set(common_args COMMAND $<TARGET_FILE:mpi_features> --no-skipped-summary --no-version)

  # add per-file tests
  foreach(f ${files_with_output})
      doctest_add_test(NAME ${f} ${common_args} -sf=*${f})
  endforeach()

  # add this separately since it shouldn't have output compared to reference output - due to concurrency
  # not adding it for MinGW since it crashes when using mingw-w64-x86_64-8.1.0-release-posix-seh-rt_v6-rev0
  # (also disabled for old XCode builds on travis where there is no thread_local support and this is defined in the build matrix)
  if(NOT MINGW AND NOT DEFINED DOCTEST_THREAD_LOCAL)
      doctest_add_test(NO_OUTPUT NAME concurrency.cpp ${common_args} -sf=*concurrency.cpp -d) # duration: there is no output anyway
  endif()

  # queries
  doctest_add_test(NAME version           COMMAND $<TARGET_FILE:mpi_features> -v)
  doctest_add_test(NAME help              ${common_args} -h)
  doctest_add_test(NO_OUTPUT NAME outfile ${common_args} -c   -out=temp) # just to exercise the output option
  doctest_add_test(NAME count             ${common_args} -c   -sf=*coverage*)
  doctest_add_test(NAME list_test_cases   ${common_args} -ltc -sf=*coverage*)
  doctest_add_test(NAME list_test_suites  ${common_args} -lts -sf=*coverage*)
  doctest_add_test(NAME list_reporters    ${common_args} -lr  -sf=*coverage*)

  # options
  doctest_add_test(NAME all_binary  ${common_args} -tc=all?binary* -s) # print all binary asserts - for getAssertString()
  doctest_add_test(NAME abort_after ${common_args} -aa=2 -e=off   -sf=*coverage*) # abort after 2 assert fails and parse a negative
  doctest_add_test(NAME first_last  ${common_args} -f=2 -l=4      -sf=*coverage*) # run a range
  doctest_add_test(NAME filter_1    ${common_args} -ts=none) # should filter out all
  # -order-by=name to avoid different output depending on the compiler used. See https://github.com/onqtam/doctest/issues/287
  doctest_add_test(NAME filter_2    COMMAND $<TARGET_FILE:mpi_features>   -tse=* -nv -order-by=name) # should filter out all + print skipped
  doctest_add_test(NAME filter_3    ${common_args} -sc=from*,sc* -sce=sc2 -sf=*subcases*) # enter a specific subcase - sc1
  doctest_add_test(NAME order_1     ${common_args} -ob=suite -ns          -sf=*test_cases_and_suites*)
  doctest_add_test(NAME order_2     ${common_args} -ob=name               -sf=*test_cases_and_suites*)
  doctest_add_test(NAME order_3     ${common_args} -ob=rand               -sfe=*) # exclude everything for no output
endif()
