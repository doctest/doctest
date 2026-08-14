#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {
namespace {
// Performs equivalent check of std::fabs(lhs - rhs) <= margin
// But without the subtraction to allow for INFINITY in comparison
bool marginComparison(double lhs, double rhs, double margin) {
    return (lhs + margin >= rhs) && (rhs + margin >= lhs);
}
} // namespace

Approx::Approx(double value)
    : m_epsilon(static_cast<double>(std::numeric_limits<float>::epsilon()) * 100), m_margin(0.0), m_scale(1.0),
      m_value(value) {}

Approx Approx::operator()(double value) const {
    Approx approx(value);
    approx.epsilon(m_epsilon);
    approx.margin(m_margin);
    approx.scale(m_scale);
    return approx;
}

Approx &Approx::epsilon(double newEpsilon) {
    m_epsilon = newEpsilon;
    return *this;
}
Approx &Approx::margin(double newMargin) {
    m_margin = newMargin;
    return *this;
}
Approx &Approx::scale(double newScale) {
    m_scale = newScale;
    return *this;
}

bool operator==(double lhs, const Approx &rhs) {
    // Absolute margin OR scaled epsilon (Catch2-style).
    // Thanks to Richard Harris for his help refining the scaled epsilon formula
    return marginComparison(lhs, rhs.m_value, rhs.m_margin) ||
           std::fabs(lhs - rhs.m_value) <
               rhs.m_epsilon * (rhs.m_scale + std::max<double>(std::fabs(lhs), std::fabs(rhs.m_value)));
}

bool operator==(const Approx &lhs, double rhs) {
    return operator==(rhs, lhs);
}

bool operator!=(double lhs, const Approx &rhs) {
    return !operator==(lhs, rhs);
}

bool operator!=(const Approx &lhs, double rhs) {
    return !operator==(rhs, lhs);
}

bool operator<=(double lhs, const Approx &rhs) {
    return lhs < rhs.m_value || lhs == rhs;
}

bool operator<=(const Approx &lhs, double rhs) {
    return lhs.m_value < rhs || lhs == rhs;
}

bool operator>=(double lhs, const Approx &rhs) {
    return lhs > rhs.m_value || lhs == rhs;
}

bool operator>=(const Approx &lhs, double rhs) {
    return lhs.m_value > rhs || lhs == rhs;
}

bool operator<(double lhs, const Approx &rhs) {
    return lhs < rhs.m_value && lhs != rhs;
}

bool operator<(const Approx &lhs, double rhs) {
    return lhs.m_value < rhs && lhs != rhs;
}

bool operator>(double lhs, const Approx &rhs) {
    return lhs > rhs.m_value && lhs != rhs;
}

bool operator>(const Approx &lhs, double rhs) {
    return lhs.m_value > rhs && lhs != rhs;
}

String toString(const Approx &in) {
    return "Approx( " + doctest::toString(in.m_value) + " )";
}

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
