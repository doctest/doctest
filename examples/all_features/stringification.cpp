#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <string>
#include <vector>
#include <list>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation diagnostics

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

template <typename T, typename K>
struct MyType
{
    T one;
    K two;
};

template <typename T>
struct MyTypeInherited : MyType<T, unsigned>
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

namespace Bar
{
struct Foo
{
    friend bool operator==(const Foo&, const Foo&) { return false; }
};

// as a third option you may provide an overload of toString()
inline doctest::String toString(const Foo&) { return "Foo{}"; }
} // namespace Bar

// set an exception translator for MyTypeInherited<int>
REGISTER_EXCEPTION_TRANSLATOR(MyTypeInherited<int>& ex) {
    return doctest::String("MyTypeInherited<int>(") + doctest::toString(ex.one) + ", " +
           doctest::toString(ex.two) + ")";
}

TEST_CASE("all asserts should fail and show how the objects get stringified") {
    MyTypeInherited<int> bla1;
    bla1.one = 5;
    bla1.two = 4u;
    MyTypeInherited<int> bla2;
    bla2.one = 5;
    bla2.two = 6u;

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

    // lets see if this exception gets translated
    throw_if(true, bla1);
}

static doctest::String intTranslator(int ex) {
    return doctest::String("int: ") + doctest::toString(ex);
}

TEST_CASE("a test case that registers an exception translator for int and then throws one") {
    // set an exception translator for int - note that this shouldn't be done in a test case but
    // in main() or somewhere before executing the tests - but here I'm just lazy...
    doctest::registerExceptionTranslator(intTranslator);

    throw_if(true, 5);
}
