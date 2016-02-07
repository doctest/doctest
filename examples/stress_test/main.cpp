#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"
#include "stress.inl"

int main(int argc, char** argv) {
    void* params = doctest::createParams(argc, argv);
    int res = doctest::runTests(params);
    doctest::freeParams(params);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
