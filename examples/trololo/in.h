#pragma once

namespace std
{
template <class charT>
struct char_traits;
template <>
class char_traits<char>;
template <class charT, class traits>
class basic_ostream;
typedef basic_ostream<char, char_traits<char> > ostream;
}

template <typename T>
struct refWrapper
{
    const T& data;
    refWrapper(const T& in)
            : data(in) {}

    operator const T&() { return data; }
};

namespace has_insertion_operator_impl
{
typedef char no;
typedef char yes[2];

struct any_t
{
    template <typename T>
    any_t(T const&);
};

no operator<<(std::ostream const&, any_t const&);

yes& test(std::ostream&);
no   test(no);

template <typename T>
struct has_insertion_operator
{
    static std::ostream& s;
    static T const&      t;
    static bool const    value = sizeof(test(s << t)) == sizeof(yes);
};
}

template <typename T>
struct has_insertion_operator : has_insertion_operator_impl::has_insertion_operator<T>
{};

template <bool, typename T = void>
struct enable_if
{};

template <typename T>
struct enable_if<true, T>
{ typedef T type; };

void defaultPrint(std::ostream& stream);

//template <typename T>
//std::ostream& operator<<(std::ostream& stream, const T&) {
//    defaultPrint(stream);
//    return stream;
//}

#include <string>

std::ostream* createStream();
std::string   getStreamResult(std::ostream*);
void          freeStream(std::ostream*);

template <class T>
typename enable_if<has_insertion_operator<T>::type, std::string>::type stringify(const T& in) {
    std::ostream* stream = createStream();
    *stream << in;
    std::string result = getStreamResult(stream);
    freeStream(stream);
    return result;
}
