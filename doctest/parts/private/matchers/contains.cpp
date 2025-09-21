#include "doctest/parts/private/prelude.h"

namespace doctest {

Contains::Contains(const String& str) : string(str) { }

bool Contains::checkWith(const String& other) const {
    return strstr(other.c_str(), string.c_str()) != nullptr;
}

String toString(const Contains& in) {
    return "Contains( " + in.string + " )";
}

bool operator==(const String& lhs, const Contains& rhs) { return rhs.checkWith(lhs); }
bool operator==(const Contains& lhs, const String& rhs) { return lhs.checkWith(rhs); }
bool operator!=(const String& lhs, const Contains& rhs) { return !rhs.checkWith(lhs); }
bool operator!=(const Contains& lhs, const String& rhs) { return !lhs.checkWith(rhs); }

} // namespace doctest
