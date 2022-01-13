#include <doctest/doctest.h>

#include "header.h"

TEST_CASE("no headers") {
    char chs1[] = { '1', 'a', 's' };
    MESSAGE(chs1); CHECK(chs1 == nullptr);
    const char* chs2{"1as"};
    MESSAGE(chs2); CHECK(chs2 == nullptr);
    MESSAGE("1as"); CHECK("1as" == nullptr);

    int ints[] = { 0, 1, 1, 2, 3, 5, 8, 13 };
    MESSAGE(ints); CHECK(ints == nullptr);

    char* cptr = (char*)(ints + 4);
    CHECK(cptr == nullptr);

    enum Test {
        A = 0, B, C = 100,
    };
    MESSAGE(A); CHECK(A == C);
}

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <limits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation diagnostics

// the standard forbids writing in the std namespace but it works on all compilers
namespace std
{
template <typename T>
ostream& operator<<(ostream& stream, const vector<T>& in) {
    stream << "[";
    for (size_t i = 0; i < in.size(); ++i) {
        if (i != 0) { stream << ", "; }
        stream << in[i];
    }
    stream << "]";
    return stream;
}
}

// as an alternative you may write a specialization of doctest::StringMaker
namespace doctest 
{ namespace detail {
template <typename T>
struct StringStream<std::list<T>>
{
    static void convert(std::ostream* s, const std::list<T>& in) {
        *s << "[";
        for (typename std::list<T>::const_iterator it = in.begin(); it != in.end();) {
            *s << *it;
            if (++it != in.end()) { *s << ", "; }
        }
        *s << "]";
    }
};
} }

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

struct MyOtherType
{
    int data;
    friend bool operator==(const MyOtherType& l, const MyOtherType& r) { return l.data == r.data; }
};

// you also can use a template operator<< if your code does not use std::ostream
template <class OStream>
OStream& operator<<(OStream& stream, const MyOtherType& in) {
    stream << "MyOtherType: " << in.data;
    return stream;
}

} // namespace Bar

namespace doctest {
    namespace detail {
        template <> struct StringStream<Bar::Foo> {
            static void convert(std::ostream* s, const Bar::Foo&) {
                *s << "Foo{}";
            }
        };
    }
}

// set an exception translator for MyTypeInherited<int>
REGISTER_EXCEPTION_TRANSLATOR(MyTypeInherited<int>& ex) {
    return doctest::String("MyTypeInherited<int>(") + doctest::toString(ex.one) + ", " +
           doctest::toString(ex.two) + ")";
}

TEST_CASE("all asserts should fail and show how the objects get stringified") {
    MyTypeInherited<int> bla1;
    bla1.one = 5;
    bla1.two = 4u;

    Bar::Foo f1;
    MESSAGE(f1);
    Bar::Foo f2;
    CHECK(f1 == f2);

    // std::string already has an operator<< working with std::ostream
    std::string dummy = "omg";

    MESSAGE(dummy);

    CHECK(dummy == "tralala"); // should fail
    CHECK("tralala" == dummy); // should fail

    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    MESSAGE(vec1);

    std::vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(4);

    CHECK(vec1 == vec2);

    std::list<int> lst_1;
    lst_1.push_back(1);
    lst_1.push_back(42);
    lst_1.push_back(3);

    MESSAGE(lst_1);

    std::list<int> lst_2;
    lst_2.push_back(1);
    lst_2.push_back(2);
    lst_2.push_back(666);

    CHECK(lst_1 == lst_2);

    {
        Bar::MyOtherType s1 {42};
        Bar::MyOtherType s2 {666};
        INFO("s1=", s1, " s2=", s2);
        CHECK(s1 == s2);
        CHECK_MESSAGE(s1 == s2, s1, " is not really ", s2);
    }

    CHECK(doctest::IsNaN<double>(0.5));
    CHECK(doctest::IsNaN<float>(std::numeric_limits<float>::infinity()));
    // can't test actual nan because it's implementation defined

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
