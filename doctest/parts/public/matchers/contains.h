#ifndef DOCTEST_PARTS_PUBLIC_MATCHERS_CONTAINS
#define DOCTEST_PARTS_PUBLIC_MATCHERS_CONTAINS

#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {

class DOCTEST_INTERFACE Contains {
public:
    explicit Contains(const String& string);

    bool checkWith(const String& other) const;

    String string;
};

DOCTEST_INTERFACE String toString(const Contains& in);

DOCTEST_INTERFACE bool operator==(const String& lhs, const Contains& rhs);
DOCTEST_INTERFACE bool operator==(const Contains& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator!=(const String& lhs, const Contains& rhs);
DOCTEST_INTERFACE bool operator!=(const Contains& lhs, const String& rhs);

} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_MATCHERS_CONTAINS
