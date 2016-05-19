//#define DOCTEST_CONFIG_DISABLE

#include "doctest.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
//#include <ostream>

//template <typename T>
//std::ostream& operator<<(std::ostream& s, const std::vector<T>& in) {
//    s << "[";
//    for(size_t i = 0; i < in.size(); ++i)
//        if(i < in.size() - 1)
//            s << in[i] << ", ";
//        else
//            s << in[i];
//    s << "]";
//    return s;
//}

#include <algorithm>
#include <limits>
#include <cmath>



SCENARIO("[math]vectors can be sized and resized") {

    GIVEN("A vector with some items") {
        std::vector<int> v(5);

        REQUIRE(v.size() == 5u);
        REQUIRE(v.capacity() >= 5u);

        WHEN("the size is increased") {
            v.resize(10);
        
            THEN("the size and capacity change") {
                REQUIRE(v.size() == 10u);
                REQUIRE(v.capacity() >= 10u);
            }
        }
        WHEN("the size is reduced") {
            v.resize(0);
        
            THEN("the size changes but not capacity") {
                REQUIRE(v.size() == 0u);
                REQUIRE(v.capacity() >= 5u);
            }
        }
        WHEN("more capacity is reserved") {
            v.reserve(10);
        
            THEN("the capacity changes but not the size") {
                REQUIRE(v.size() == 5u);
                REQUIRE(v.capacity() >= 10u);
            }
        }
        WHEN("less capacity is reserved") {
            v.reserve(0);
        
            THEN("neither size nor capacity are changed") {
                REQUIRE(v.size() == 5u);
                REQUIRE(v.capacity() >= 5u);
            }
        }
    }
}

//TEST_CASE("zzz") {
//    //CHECK(std::string("OMG2") == std::string("OMG"));
//
//    std::vector<int> vec1;
//    vec1.push_back(1);
//    vec1.push_back(2);
//    vec1.push_back(3);
//
//    std::vector<int> vec2;
//    vec2.push_back(1);
//    vec2.push_back(2);
//    vec2.push_back(4);
//
//    //CHECK(vec1 == vec2);
//}

int main(int argc, char** argv) {
    // initialize
    doctest::Context context(argc, argv);

    // overrides
    context.setOption("dt-case-sensitive", true);
    context.setOption("no-breaks", true);
    //context.setOption("success", true);
    //context.setOption("no-throw", true);
    //context.setOption("dt-no-colors", true);
    //context.addFilter("test-case", "zzz");
    //context.setOption("abort-after", 1);
    //context.addFilter("suite", "randomness");
    //context.setOption("sort", "rand");
    //context.setOption("rand-seed", 435);

    // run
    int res = context.run();

    if(context.shouldExit())
        return res;

#if defined(WITH_PAUSE)
    system("pause");
#endif // WITH_PAUSE

    return res;
}
