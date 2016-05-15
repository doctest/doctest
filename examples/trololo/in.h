#pragma once

class String
{
    char* m_str;

    void copy(const String& other);

public:
    String(const char* in = "");
    String(const String& other);
    ~String();

    String& operator=(const String& other);

    String operator+(const String& other) const;
    String& operator+=(const String& other);

    char& operator[](unsigned pos) { return m_str[pos]; }
    const char& operator[](unsigned pos) const { return m_str[pos]; }

    char*       c_str() { return m_str; }
    const char* c_str() const { return m_str; }
};

#ifdef __clang__
#include <ciso646>
#endif // __clang__

#ifdef _LIBCPP_VERSION
#include <iosfwd>
#else // _LIBCPP_VERSION
#ifndef DOCTEST_CONFIG_USE_IOSFWD
namespace std
{
template <class charT>
struct char_traits;
template <>
struct char_traits<char>;
template <class charT, class traits>
class basic_ostream;
typedef basic_ostream<char, char_traits<char> > ostream;
}
#else // DOCTEST_CONFIG_USE_IOSFWD
#include <iosfwd>
#endif // DOCTEST_CONFIG_USE_IOSFWD
#endif // _LIBCPP_VERSION

namespace has_insertion_operator_impl
{
typedef char no;
typedef char yes[2];

template <bool>
struct static_assertion;

template <>
struct static_assertion<true>
{};

template <bool in>
void           f() {
    static_assertion<in>(); }

    struct any_t {
        template <typename T>
        any_t(T const&) {
            f<false>();
        }
    };
}
has_insertion_operator_impl::no operator<<(std::ostream const&, has_insertion_operator_impl::any_t const&);
namespace has_insertion_operator_impl {
    yes& test(std::ostream&);
    no   test(no);

    template <typename T>
    struct has_insertion_operator
    {
        static std::ostream& s;
        static T const&      t;
        static bool const    value = sizeof(test(s << t)) == sizeof(yes);
    };
} // namespace has_insertion_operator_impl

template <typename T>
struct has_insertion_operator : has_insertion_operator_impl::has_insertion_operator<T>
{};

template <bool, typename T = void>
struct my_enable_if
{};

template <typename T>
struct my_enable_if<true, T>
{ typedef T value; };

std::ostream* createStream();
String   getStreamResult(std::ostream*);
void          freeStream(std::ostream*);

template <class T>
typename my_enable_if<has_insertion_operator<T>::value, String>::value stringify(const T& in) {
    std::ostream* stream = createStream();
    *stream << in;
    String result = getStreamResult(stream);
    freeStream(stream);
    return result;
}

template <class T>
typename my_enable_if<!has_insertion_operator<T>::value, String>::value stringify(const T&) {
    return "{?}";
}
