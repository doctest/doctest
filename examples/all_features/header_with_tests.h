#include "doctest.h"

#include <iostream>
#include <vector>
using namespace std;

typedef doctest::Types<char, short, int> the_types;

TEST_CASE_TEMPLATE("signed integers stuff", T, the_types) {
    T var = T();
    --var;
    CHECK(var == -1);
}




TYPE_TO_STRING(std::vector<int>);








TEST_CASE_TEMPLATE_DEFINE("default construction", T, test_id) {
    T var = T();
    CHECK(var == T());
}

TEST_CASE_TEMPLATE_INSTANTIATE(test_id, the_types);
TEST_CASE_TEMPLATE_INSTANTIATE(test_id, doctest::Types<float, double>);
