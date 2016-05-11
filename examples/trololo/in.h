#pragma once

#include <string>
//#include <iosfwd>
#include <ciso646>
using namespace std;

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

namespace has_insertion_operator_impl {
    typedef char no;
    typedef char yes[2];

    template<bool>
    struct static_assertion;

    template<>
    struct static_assertion<true> {};

    template<bool in>
    void f() { static_assertion<in>(); }

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
std::string   getStreamResult(std::ostream*);
void          freeStream(std::ostream*);

template <class T>
typename my_enable_if<has_insertion_operator<T>::value, std::string>::value stringify(const T& in) {
    std::ostream* stream = createStream();
    *stream << in;
    std::string result = getStreamResult(stream);
    freeStream(stream);
    return result;
}

//template <template <typename, typename> class T, typename T0, typename T1>
//typename my_enable_if<has_insertion_operator<T<T0, T1>>::value, std::string>::value stringify(const T<T0, T1>& in) {
//    std::ostream* stream = createStream();
//    *stream << in;
//    std::string result = getStreamResult(stream);
//    freeStream(stream);
//    return result;
//}

template <class T>
typename my_enable_if<!has_insertion_operator<T>::value, std::string>::value stringify(const T&) {
    return "{?}";
}

//template <template <typename, typename> class T, typename T0, typename T1>
//typename my_enable_if<!has_insertion_operator<T<T0, T1>>::value, std::string>::value stringify(const T<T0, T1>& in) {
//    return "{?}";
//}
