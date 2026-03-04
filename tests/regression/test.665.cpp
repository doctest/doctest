#include <doctest/doctest.h>


TEST_CASE("Pointer equality") {
  const auto a = new int;
  const auto b = a;

  CHECK(a == b);
  delete a;
}
