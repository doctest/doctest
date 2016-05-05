#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <vector>

namespace doctest
{
    static String stringify(ADL_helper, const std::string& in) {
        return String("\"") + in.c_str() + "\"";
    }

    template <typename T>
    String stringify(ADL_helper, const std::vector<T>& in) {
        String out("vector[");
        for(unsigned i = 0; i < in.size(); ++i)
            out += stringify(ADL_helper(), in[i]) + (i + 1 == in.size() ? "]" : ", ");
        return out;
    }
} // namespace doctest

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
