#include <doctest/doctest.h>
#include <doctest/parts/private/exception_translator.h>
#include <string>
#include <type_traits>

namespace {

template <typename T>
doctest::String translate(T failure) try {
  throw failure;
} catch (...) {
  return doctest::detail::translateActiveException();
}

} // namespace


TEST_CASE("Throwing a null const char *") {
  const char *failure = nullptr;
  const auto reason = translate(failure);
  CHECK(reason == "");
}

TEST_CASE("Throwing an untyped nullptr") {
  const auto reason = translate(nullptr);
  CHECK(reason == "");
}
