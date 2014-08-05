#include "doctest.h"

doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}
doctest{}



struct Z {
    static void check() { int a = 5; }
};

DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(Z);

#include "u.h"

DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(X);
DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(X);
DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(X);
DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(X);

doctest { int a = 5; }
