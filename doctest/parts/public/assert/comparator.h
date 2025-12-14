#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail  {

    // clang-format off
#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    template<class T>               struct decay_array       { using type = T; };
    template<class T, unsigned N>   struct decay_array<T[N]> { using type = T*; };
    template<class T>               struct decay_array<T[]>  { using type = T*; };

    template<class T>   struct not_char_pointer              { static DOCTEST_CONSTEXPR int value = 1; };
    template<>          struct not_char_pointer<char*>       { static DOCTEST_CONSTEXPR int value = 0; };
    template<>          struct not_char_pointer<const char*> { static DOCTEST_CONSTEXPR int value = 0; };

    template<class T> struct can_use_op : public not_char_pointer<typename decay_array<T>::type> {};
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    // clang-format on

    // clang-format off
#ifndef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_COMPARISON_RETURN_TYPE bool
#else // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_COMPARISON_RETURN_TYPE typename types::enable_if<can_use_op<L>::value || can_use_op<R>::value, bool>::type
    inline bool eq(const char* lhs, const char* rhs) { return String(lhs) == String(rhs); }
    inline bool ne(const char* lhs, const char* rhs) { return String(lhs) != String(rhs); }
    inline bool lt(const char* lhs, const char* rhs) { return String(lhs) <  String(rhs); }
    inline bool gt(const char* lhs, const char* rhs) { return String(lhs) >  String(rhs); }
    inline bool le(const char* lhs, const char* rhs) { return String(lhs) <= String(rhs); }
    inline bool ge(const char* lhs, const char* rhs) { return String(lhs) >= String(rhs); }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    // clang-format on

#define DOCTEST_RELATIONAL_OP(name, op)                                                            \
    template <typename L, typename R>                                                              \
    DOCTEST_COMPARISON_RETURN_TYPE name(const DOCTEST_REF_WRAP(L) lhs,                             \
                                        const DOCTEST_REF_WRAP(R) rhs) {                           \
        return lhs op rhs;                                                                         \
    }

    DOCTEST_RELATIONAL_OP(eq, ==)
    DOCTEST_RELATIONAL_OP(ne, !=)
    DOCTEST_RELATIONAL_OP(lt, <)
    DOCTEST_RELATIONAL_OP(gt, >)
    DOCTEST_RELATIONAL_OP(le, <=)
    DOCTEST_RELATIONAL_OP(ge, >=)

#ifndef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CMP_EQ(l, r) l == r
#define DOCTEST_CMP_NE(l, r) l != r
#define DOCTEST_CMP_GT(l, r) l > r
#define DOCTEST_CMP_LT(l, r) l < r
#define DOCTEST_CMP_GE(l, r) l >= r
#define DOCTEST_CMP_LE(l, r) l <= r
#else // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CMP_EQ(l, r) eq(l, r)
#define DOCTEST_CMP_NE(l, r) ne(l, r)
#define DOCTEST_CMP_GT(l, r) gt(l, r)
#define DOCTEST_CMP_LT(l, r) lt(l, r)
#define DOCTEST_CMP_GE(l, r) ge(l, r)
#define DOCTEST_CMP_LE(l, r) le(l, r)
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

    namespace binaryAssertComparison {
        enum Enum
        {
            eq = 0,
            ne,
            gt,
            lt,
            ge,
            le
        };
    } // namespace binaryAssertComparison

    // clang-format off
    template <int, class L, class R> struct RelationalComparator     { bool operator()(const DOCTEST_REF_WRAP(L),     const DOCTEST_REF_WRAP(R)    ) const { return false;        } };

#define DOCTEST_BINARY_RELATIONAL_OP(n, op) \
    template <class L, class R> struct RelationalComparator<n, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return op(lhs, rhs); } };
    // clang-format on

    DOCTEST_BINARY_RELATIONAL_OP(0, doctest::detail::eq)
    DOCTEST_BINARY_RELATIONAL_OP(1, doctest::detail::ne)
    DOCTEST_BINARY_RELATIONAL_OP(2, doctest::detail::gt)
    DOCTEST_BINARY_RELATIONAL_OP(3, doctest::detail::lt)
    DOCTEST_BINARY_RELATIONAL_OP(4, doctest::detail::ge)
    DOCTEST_BINARY_RELATIONAL_OP(5, doctest::detail::le)

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
