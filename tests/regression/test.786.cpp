#include <doctest/doctest.h>
#include <cstdint>

TEST_CASE("Bitfield") {
    struct S {
        uint32_t a : 31;
        uint32_t b : 1;
    };

    S s;
    s.a = 0;
    s.b = 1;
    CHECK(s.a == 0);
}

TEST_CASE("Packed struct") {
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wpacked")
#if !defined(WIN32)
    struct __attribute__((packed)) P {
        int i;
    };
#else
#pragma pack(push, 1)
    struct P {
        int i;
    };
#pragma pack(pop)
#endif
    DOCTEST_CLANG_SUPPRESS_WARNING_POP

    P p;
    p.i = 0;
    CHECK(p.i == 0);
}
