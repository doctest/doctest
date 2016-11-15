#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <vector>
#include <list>

#include <sstream>

// the standard forbids writing in the std namespace but it works on all compilers
namespace std
{
template <typename T>
ostream& operator<<(ostream& stream, const vector<T>& in) {
    stream << "[";
    for(size_t i = 0; i < in.size(); ++i)
        if(i < in.size() - 1)
            stream << in[i] << ", ";
        else
            stream << in[i];
    stream << "]";
    return stream;
}
}

// as an alternative you may write a specialization of doctest::StringMaker
namespace doctest
{
template <typename T>
struct StringMaker<std::list<T> >
{
    static String convert(const std::list<T>& in) {
        std::ostringstream oss;

        oss << "[";
        for(typename std::list<T>::const_iterator it = in.begin(); it != in.end(); ++it)
            oss << *it << ", ";
        oss << "]";

        return oss.str().c_str();
    }
};
}

// to silence GCC warnings when inheriting from the class MyType which has no virtual destructor
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif // __GNUC__

template <typename T, typename K>
struct MyType
{
    T one;
    K two;
};

template <typename T>
struct MyTypeInherited : MyType<T, float>
{};

template <typename T, typename K>
bool operator==(const MyType<T, K>& lhs, const MyType<T, K>& rhs) {
    return lhs.one == rhs.one && lhs.two == rhs.two;
}

template <typename T, typename K>
std::ostream& operator<<(std::ostream& stream, const MyType<T, K>& in) {
    stream << "[" << in.one << ", " << in.two << "]";
    return stream;
}

namespace Bar {
struct Foo
{};
static bool operator==(const Foo&, const Foo&) { return false; }

doctest::String toString(const Foo&); // to silence -Wmissing-declarations
// as a third option you may provide an overload of toString()
doctest::String toString(const Foo&) {
    return "Foo{}";
}
} // namespace Bar

TEST_CASE("the only test") {
    MyTypeInherited<int> bla1;
    bla1.one = 5;
    bla1.two = 4.0f;
    MyTypeInherited<int> bla2;
    bla2.one = 5;
    bla2.two = 6.0f;

    Bar::Foo f1;
    Bar::Foo f2;
    CHECK(f1 == f2);

    // std::string already has an operator<< working with std::ostream
    std::string dummy1 = "omg";
    std::string dummy2 = "tralala";

    CHECK(dummy1 == dummy2);

    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    std::vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(4);

    CHECK(vec1 == vec2);

    std::list<int> lst_1;
    lst_1.push_back(1);
    lst_1.push_back(42);
    lst_1.push_back(3);

    std::list<int> lst_2;
    lst_2.push_back(1);
    lst_2.push_back(2);
    lst_2.push_back(666);

    CHECK(lst_1 == lst_2);
}
