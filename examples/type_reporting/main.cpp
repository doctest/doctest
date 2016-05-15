#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <vector>
#include <ostream>

template <typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& in) {
    s << "[";
    for(size_t i = 0; i < in.size(); ++i)
        if(i < in.size() - 1)
            s << in[i] << ", ";
        else
            s << in[i];
    s << "]";
    return s;
}

TEST_CASE("the only test") {
    std::string dummy1 = "omg";
    std::string dummy2 = "tralala";

    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    std::vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(4);

    CHECK(4 == 3);

    CHECK(dummy1 == dummy2);

    CHECK(vec1 == vec2);
}
