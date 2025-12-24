#ifndef DOCTEST_PARTS_PUBLIC_PATH
#define DOCTEST_PARTS_PUBLIC_PATH

#include "doctest/parts/public/config.h"
#include "doctest/parts/public/warnings.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {

DOCTEST_INTERFACE const char* skipPathFromFilename(const char* file);

} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_PATH
