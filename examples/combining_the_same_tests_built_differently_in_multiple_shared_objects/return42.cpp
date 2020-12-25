#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest/doctest.h"

#define RETURN_42
#include "foo.h"

DOCTEST_SYMBOL_EXPORT void return42_cpp_force_link() {}
