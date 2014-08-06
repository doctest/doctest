#include "doctest.h"

#include <cstdio>

doctest(Asd) { printf("%s\n", __func__); }
doctest() { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }
doctest(asdfa) { printf("%s\n", __func__); }
doctest(Asd) { printf("%s\n", __func__); }
doctest(adffd) { printf("%s\n", __func__); }
doctest(qweqq) { printf("%s\n", __func__); }
doctest() { printf("%s\n", __func__); }

struct MILF{};
doctest_fixture(MILF, ) { printf("%s\n", __func__); }
doctest_fixture(MILF, qwe) { printf("%s\n", __func__); }
doctest_fixture(MILF, q) { printf("%s\n", __func__); }
doctest_fixture(MILF, dsf) { printf("%s\n", __func__); }

