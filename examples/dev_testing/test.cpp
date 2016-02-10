#include "doctest.h"

#include <cstdio>

testsuite(test);

test(ttt)
{
    printf("test!\n");
}

struct F {
    F()
    {
        printf("ctor!\n");
    }
    ~F()
    {
        printf("dtor...\n");
    }
};

fixture_noname(F)
{
    printf("    fixturing!\n");
}

testsuite_end;


test(__LINE__) {}
test(__LINE__) {}