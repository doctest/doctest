#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest/extensions/doctest_mpi.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  doctest::Context ctx;
  ctx.setOption("reporters", "MpiConsoleReporter");
  ctx.setOption("reporters", "MpiFileReporter");
  ctx.setOption("force-colors", true);
  ctx.applyCommandLine(argc, argv);

  int test_result = ctx.run();

  MPI_Finalize();

  return test_result;
}
