//#define DOCTEST_CONFIG_NO_STATIC_ASSERT
#include "parts/doctest_fwd.h"

#include <iostream>
#include <typeinfo>
using namespace std;

namespace doctest
{
namespace detail
{
    class NullType
    {};

    template <class T, class U>
    struct Typelist
    {
        typedef T Head;
        typedef U Tail;
    };

    template <class TList, class T>
    struct Append;

    template <>
    struct Append<NullType, NullType>
    { typedef NullType Result; };

    template <class T>
    struct Append<NullType, T>
    { typedef Typelist<T, NullType> Result; };

    template <class Head, class Tail>
    struct Append<NullType, Typelist<Head, Tail> >
    { typedef Typelist<Head, Tail> Result; };

    template <class Head, class Tail, class T>
    struct Append<Typelist<Head, Tail>, T>
    { typedef Typelist<Head, typename Append<Tail, T>::Result> Result; };

    template <class TList>
    struct Reverse;

    template <>
    struct Reverse<NullType>
    { typedef NullType Result; };

    template <class Head, class Tail>
    struct Reverse<Typelist<Head, Tail> >
    { typedef typename Append<typename Reverse<Tail>::Result, Head>::Result Result; };

    template <typename T1 = NullType, typename T2 = NullType, typename T3 = NullType,
              typename T4 = NullType, typename T5 = NullType, typename T6 = NullType,
              typename T7 = NullType, typename T8 = NullType, typename T9 = NullType,
              typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
              typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
              typename T16 = NullType, typename T17 = NullType, typename T18 = NullType,
              typename T19 = NullType, typename T20 = NullType, typename T21 = NullType,
              typename T22 = NullType, typename T23 = NullType, typename T24 = NullType,
              typename T25 = NullType, typename T26 = NullType, typename T27 = NullType,
              typename T28 = NullType, typename T29 = NullType, typename T30 = NullType,
              typename T31 = NullType, typename T32 = NullType, typename T33 = NullType,
              typename T34 = NullType, typename T35 = NullType, typename T36 = NullType,
              typename T37 = NullType, typename T38 = NullType, typename T39 = NullType,
              typename T40 = NullType, typename T41 = NullType, typename T42 = NullType,
              typename T43 = NullType, typename T44 = NullType, typename T45 = NullType,
              typename T46 = NullType, typename T47 = NullType, typename T48 = NullType,
              typename T49 = NullType, typename T50 = NullType, typename T51 = NullType,
              typename T52 = NullType, typename T53 = NullType, typename T54 = NullType,
              typename T55 = NullType, typename T56 = NullType, typename T57 = NullType,
              typename T58 = NullType, typename T59 = NullType, typename T60 = NullType>
    struct MakeTypelist
    {
    private:
        typedef typename MakeTypelist<
                T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19,
                T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,
                T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53,
                T54, T55, T56, T57, T58, T59, T60>::Result TailResult;

    public:
        typedef Typelist<T1, TailResult> Result;
    };

    template <>
    struct MakeTypelist<>
    { typedef NullType Result; };

    // type of recursive function
    template <class TList, class Callable>
    struct ForEachTypeImpl;

    // Recursion rule
    template <class Head, class Tail, class Callable>
    struct ForEachTypeImpl<Typelist<Head, Tail>, Callable> : public ForEachTypeImpl<Tail, Callable>
    {
        enum
        {
            value = 1 + ForEachTypeImpl<Tail, Callable>::value
        };

        ForEachTypeImpl(Callable& callable)
                : ForEachTypeImpl<Tail, Callable>(callable) {
            callable.template operator()<value, Head>();
        }
    };

    // Recursion end
    template <class Head, class Callable>
    struct ForEachTypeImpl<Typelist<Head, NullType>, Callable>
    {
    public:
        enum
        {
            value = 0
        };

        ForEachTypeImpl(Callable& callable) { callable.template operator()<value, Head>(); }
    };

    struct OrderPolicyForward;
    struct OrderPolicyBackward;

    template <class TList, class Callable, class OrderPolicy = OrderPolicyForward>
    struct ForEachType;

    template <class TList, class Callable>
    struct ForEachType<TList, Callable, OrderPolicyForward>
            : public ForEachTypeImpl<typename Reverse<TList>::Result, Callable>
    {
        ForEachType(Callable& callable)
                : ForEachTypeImpl<typename Reverse<TList>::Result, Callable>(callable) {}
    };

    template <class TList, class Callable>
    struct ForEachType<TList, Callable, OrderPolicyBackward>
            : public ForEachTypeImpl<TList, Callable>
    {
        ForEachType(Callable& callable)
                : ForEachTypeImpl<TList, Callable>(callable) {}
    };

    template <typename T>
    inline const char* type_to_string() {
        return "<?>";
    }
} // namespace detail
} // namespace doctest

#ifdef DOCTEST_CONFIG_WITH_VARIADIC_MACROS
#define DOCTEST_TYPE_TO_STRING(...)                                                                \
    namespace doctest                                                                              \
    {                                                                                              \
        namespace detail                                                                           \
        {                                                                                          \
            template <>                                                                            \
            inline const char* type_to_string<__VA_ARGS__>() {                                     \
                return "<" #__VA_ARGS__ ">";                                                       \
            }                                                                                      \
        }                                                                                          \
    }
#else // DOCTEST_CONFIG_WITH_VARIADIC_MACROS
#define DOCTEST_TYPE_TO_STRING(x)                                                                  \
    namespace doctest                                                                              \
    {                                                                                              \
        namespace detail                                                                           \
        {                                                                                          \
            template <>                                                                            \
            inline const char* type_to_string<x>() {                                               \
                return "<" #x ">";                                                                 \
            }                                                                                      \
        }                                                                                          \
    }
#endif // DOCTEST_CONFIG_WITH_VARIADIC_MACROS

#define DOCTEST_TEST_CASE_TEMPLATE_IMPL(name, type, types, anon)                                   \
    template <typename T>                                                                          \
    inline void anon();                                                                            \
    struct DOCTEST_CAT(anon, FUNCTOR)                                                              \
    {                                                                                              \
        template <int Index, typename Type>                                                        \
        void          operator()() {                                                               \
            doctest::detail::regTest(anon<Type>, __LINE__, __FILE__, name,                         \
                                     doctest_detail_test_suite_ns::getCurrentTestSuite(),          \
                                     doctest::detail::type_to_string<Type>(), Index);              \
        }                                                                                          \
    };                                                                                             \
    inline int DOCTEST_CAT(anon, REG_FUNC)() {                                                     \
        DOCTEST_CAT(anon, FUNCTOR) registrar;                                                      \
        doctest::detail::ForEachType<types, DOCTEST_CAT(anon, FUNCTOR)> doIt(registrar);           \
        return 0;                                                                                  \
    }                                                                                              \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_CAT(anon, DUMMY)) = DOCTEST_CAT(anon, REG_FUNC)();          \
    DOCTEST_GLOBAL_NO_WARNINGS_END()                                                               \
    template <typename type>                                                                       \
    inline void anon()

#define DOCTEST_TEST_CASE_TEMPLATE(name, type, types)                                              \
    DOCTEST_TEST_CASE_TEMPLATE_IMPL(name, type, types, DOCTEST_ANONYMOUS(_DOCTEST_ANON_TMP_))

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(name, type, id, anon)                               \
    template <typename T>                                                                          \
    inline void anon();                                                                            \
    struct DOCTEST_CAT(id, _FUNCTOR)                                                               \
    {                                                                                              \
        int m_line;                                                                                \
        DOCTEST_CAT(id, _FUNCTOR)                                                                  \
        (int line)                                                                                 \
                : m_line(line) {}                                                                  \
        template <int Index, typename Type>                                                        \
        void          operator()() {                                                               \
            doctest::detail::regTest(anon<Type>, __LINE__, __FILE__, name,                         \
                                     doctest_detail_test_suite_ns::getCurrentTestSuite(),          \
                                     doctest::detail::type_to_string<Type>(),                      \
                                     m_line * 1000 + Index);                                       \
        }                                                                                          \
    };                                                                                             \
    template <typename type>                                                                       \
    inline void anon()

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE(name, type, id)                                          \
    DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(name, type, id, DOCTEST_ANONYMOUS(_DOCTEST_ANON_TMP_))

#define DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, types, anon)                               \
    static int DOCTEST_CAT(anon, REG_FUNC)() {                                                     \
        DOCTEST_CAT(id, _FUNCTOR) registrar(__LINE__);                                             \
        doctest::detail::ForEachType<types, DOCTEST_CAT(id, _FUNCTOR)> doIt(registrar);            \
        return 0;                                                                                  \
    }                                                                                              \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_CAT(anon, DUMMY)) = DOCTEST_CAT(anon, REG_FUNC)();          \
    DOCTEST_GLOBAL_NO_WARNINGS_END()

#define DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE(id, types)                                          \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, types, DOCTEST_ANONYMOUS(_DOCTEST_ANON_TMP_))

#define TYPE_TO_STRING DOCTEST_TYPE_TO_STRING
#define TEST_CASE_TEMPLATE DOCTEST_TEST_CASE_TEMPLATE
#define TEST_CASE_TEMPLATE_DEFINE DOCTEST_TEST_CASE_TEMPLATE_DEFINE
#define TEST_CASE_TEMPLATE_INSTANTIATE DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE

TYPE_TO_STRING(int)
TYPE_TO_STRING(char)
TYPE_TO_STRING(double)

typedef doctest::detail::MakeTypelist<int, char, float, double, int>::Result the_types;

template<typename first, typename second>
struct TypePair {
    typedef first A;
    typedef second B;
};

typedef doctest::detail::MakeTypelist<
    TypePair<int, char>,
    TypePair<char, int>
>::Result pairs;

TYPE_TO_STRING(TypePair<char, int>)

TEST_CASE_TEMPLATE("trololo", T, pairs) {
    CHECK(sizeof(typename T::A) == static_cast<unsigned>(4));
    CHECK(sizeof(typename T::B) == static_cast<unsigned>(4));
    //T a = T();
    //++a;
    //CHECK(doctest::Approx(a) == T());
}

TEST_CASE_TEMPLATE_DEFINE("separate", T, it) {
    T a = T();
    ++a;
    SUBCASE("root") {
        ++a;
        SUBCASE("1") {
            CHECK(doctest::Approx(a) == T());
        }
        SUBCASE("2") {
            CHECK(doctest::Approx(a) == T());
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(it, the_types)
TEST_CASE_TEMPLATE_INSTANTIATE(it, the_types)
