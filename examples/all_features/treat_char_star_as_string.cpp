#include <doctest/doctest.h>

#include <cstring>

struct Raii {
    char* value;

    Raii(const char* src) {
        value = new char[strlen(src) + 1];
        char* it = value;
        while (*it++ = *src++);
    }

    ~Raii() {
        delete[] value;
    }
};

TEST_CASE("Treating star char as string?") {
    const char* a = "asd";
    Raii raii(a);

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    CHECK(a == raii.value);
#else
    CHECK(a != raii.value);
#endif
}
