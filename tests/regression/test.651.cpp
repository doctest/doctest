#include <doctest/doctest.h>

namespace {

DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-member-function")
struct int_pointer {
    int *p = nullptr;
    int_pointer() = default;

    // non-const ref used in constructor
    // we don't want to accidentally construct a dangling pointer from a temporary
    int_pointer(int &i)
        : p(&i) {}

    // NOLINTNEXTLINE(readability-make-member-function-const)
    explicit operator bool() {
        return !!p;
    }
    int val() const {
        return *p;
    }
};
DOCTEST_CLANG_SUPPRESS_WARNING_POP

struct int_holder {
    int i;
    bool operator==(int_pointer p) const {
        if (!p)
            return false;
        return i == p.val();
    }
};

} // namespace

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866) // eval order
TEST_CASE("Impl cast from non-const value") {
    int_holder h{8}; // NOLINT(misc-const-correctness)
    int i = 8;

    // this comparison is safe
    // int_pointer can be implicitly constructed from this int here
    CHECK(h == i);
}
DOCTEST_MSVC_SUPPRESS_WARNING_POP
