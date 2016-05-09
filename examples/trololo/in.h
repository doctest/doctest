#pragma once

struct String
{
    char* data;

    String(const char* in = 0);
    String(const String& other);
    String& operator=(const String& other);
    ~String();

    operator char*() { return data; }
};

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
{ typedef T value; };

std::ostream* createStream();
String        getStreamResult(std::ostream*);
void          freeStream(std::ostream*);

template <class T>
typename enable_if<has_insertion_operator<T>::value, String>::value stringify(const T& in) {
    std::ostream* stream = createStream();
    *stream << in;
    String result = getStreamResult(stream);
    freeStream(stream);
    return result;
}

template <class T>
typename enable_if<!has_insertion_operator<T>::value, String>::value stringify(const T&) {
    return "{?}";
}
