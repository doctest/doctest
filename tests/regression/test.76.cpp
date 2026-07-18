#include <doctest/doctest.h>

class TestSubject {
private:
    static int nop();

    class DocTest;
    friend DocTest;
};

int TestSubject::nop() {
    return 0;
}

class TestSubject::DocTest {
public:
    TEST_CASE_CLASS("access private class member from test case") {
        REQUIRE(TestSubject::nop() == 0);
    }
};
