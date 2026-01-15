#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/exceptions.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {
namespace detail {

    DOCTEST_THREAD_LOCAL class
    {
        std::vector<std::streampos> stack;
        std::stringstream           ss;

    public:
        std::ostream* push() {
            stack.push_back(ss.tellp());
            return &ss;
        }

        String pop() {
            if (stack.empty())
                DOCTEST_INTERNAL_ERROR("TLSS was empty when trying to pop!");

            std::streampos pos = stack.back();
            stack.pop_back();
            unsigned sz = static_cast<unsigned>(ss.tellp() - pos);
            ss.rdbuf()->pubseekpos(pos, std::ios::in | std::ios::out);
            return String(ss, sz);
        }
    } g_oss;

    std::ostream* tlssPush() {
        return g_oss.push();
    }

    String tlssPop() {
        return g_oss.pop();
    }

} // namespace detail

    // case insensitive strcmp
    static int stricmp(const char* a, const char* b) {
        for(;; a++, b++) {
            const int d = tolower(*a) - tolower(*b);
            if(d != 0 || !*a)
                return d;
        }
    }

    char* String::allocate(size_type sz) {
        if (sz <= last) {
            buf[sz] = '\0';
            setLast(last - sz);
            return buf;
        } else {
            setOnHeap();
            data.size = sz;
            data.capacity = data.size + 1;
            data.ptr = new char[data.capacity];
            data.ptr[sz] = '\0';
            return data.ptr;
        }
    }

    void String::setOnHeap() noexcept { *reinterpret_cast<unsigned char*>(&buf[last]) = 128; }
    void String::setLast(size_type in) noexcept { buf[last] = char(in); }
    void String::setSize(size_type sz) noexcept {
        if (isOnStack()) { buf[sz] = '\0'; setLast(last - sz); }
        else { data.ptr[sz] = '\0'; data.size = sz; }
    }

    void String::copy(const String& other) {
        if(other.isOnStack()) {
            memcpy(buf, other.buf, len);
        } else {
            memcpy(allocate(other.data.size), other.data.ptr, other.data.size);
        }
    }

    String::String() noexcept {
        buf[0] = '\0';
        setLast();
    }

    String::~String() {
        if(!isOnStack())
            delete[] data.ptr;
    } // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)

    String::String(const char* in)
            : String(in, strlen(in)) {}

    String::String(const char* in, size_type in_size) {
        memcpy(allocate(in_size), in, in_size);
    }

    String::String(std::istream& in, size_type in_size) {
        in.read(allocate(in_size), in_size);
    }

    String::String(const String& other) { copy(other); }

    String& String::operator=(const String& other) {
        if(this != &other) {
            if(!isOnStack())
                delete[] data.ptr;

            copy(other);
        }

        return *this;
    }

    String& String::operator+=(const String& other) {
        const size_type my_old_size = size();
        const size_type other_size  = other.size();
        const size_type total_size  = my_old_size + other_size;
        if(isOnStack()) {
            if(total_size < len) {
                // append to the current stack space
                memcpy(buf + my_old_size, other.c_str(), other_size + 1);
                // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
                setLast(last - total_size);
            } else {
                // alloc new chunk
                char* temp = new char[total_size + 1];
                // copy current data to new location before writing in the union
                memcpy(temp, buf, my_old_size); // skip the +1 ('\0') for speed
                // update data in union
                setOnHeap();
                data.size     = total_size;
                data.capacity = data.size + 1;
                data.ptr      = temp;
                // transfer the rest of the data
                memcpy(data.ptr + my_old_size, other.c_str(), other_size + 1);
            }
        } else {
            if(data.capacity > total_size) {
                // append to the current heap block
                data.size = total_size;
                memcpy(data.ptr + my_old_size, other.c_str(), other_size + 1);
            } else {
                // resize
                data.capacity *= 2;
                if(data.capacity <= total_size)
                    data.capacity = total_size + 1;
                // alloc new chunk
                char* temp = new char[data.capacity];
                // copy current data to new location before releasing it
                memcpy(temp, data.ptr, my_old_size); // skip the +1 ('\0') for speed
                // release old chunk
                delete[] data.ptr;
                // update the rest of the union members
                data.size = total_size;
                data.ptr  = temp;
                // transfer the rest of the data
                memcpy(data.ptr + my_old_size, other.c_str(), other_size + 1);
            }
        }

        return *this;
    }

    String::String(String&& other) noexcept {
        memcpy(buf, other.buf, len);
        other.buf[0] = '\0';
        other.setLast();
    }

    String& String::operator=(String&& other) noexcept {
        if(this != &other) {
            if(!isOnStack())
                delete[] data.ptr;
            memcpy(buf, other.buf, len);
            other.buf[0] = '\0';
            other.setLast();
        }
        return *this;
    }

    char String::operator[](size_type i) const {
        return const_cast<String*>(this)->operator[](i);
    }

    char& String::operator[](size_type i) {
        if(isOnStack())
            return reinterpret_cast<char*>(buf)[i];
        return data.ptr[i];
    }

    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wmaybe-uninitialized")
    String::size_type String::size() const {
        if(isOnStack())
            return last - (size_type(buf[last]) & 31); // using "last" would work only if "len" is 32
        return data.size;
    }
    DOCTEST_GCC_SUPPRESS_WARNING_POP

    String::size_type String::capacity() const {
        if(isOnStack())
            return len;
        return data.capacity;
    }

    String String::substr(size_type pos, size_type cnt) && {
        cnt = std::min(cnt, size() - pos);
        char* cptr = c_str();
        memmove(cptr, cptr + pos, cnt);
        setSize(cnt);
        return std::move(*this);
    }

    String String::substr(size_type pos, size_type cnt) const & {
        cnt = std::min(cnt, size() - pos);
        return String{ c_str() + pos, cnt };
    }

    String::size_type String::find(char ch, size_type pos) const {
        const char* begin = c_str();
        const char* end = begin + size();
        const char* it = begin + pos;
        for (; it < end && *it != ch; it++) { }
        if (it < end) { return static_cast<size_type>(it - begin); }
        else { return npos; }
    }

    String::size_type String::rfind(char ch, size_type pos) const {
        if (size() == 0) { return npos; }

        const char* begin = c_str();
        const char* it = begin + std::min(pos, size() - 1);
        for (; it >= begin && *it != ch; it--) { }
        if (it >= begin) { return static_cast<size_type>(it - begin); }
        else { return npos; }
    }

    int String::compare(const char* other, bool no_case) const {
        if(no_case)
            return doctest::stricmp(c_str(), other);
        return std::strcmp(c_str(), other);
    }

    int String::compare(const String& other, bool no_case) const {
        return compare(other.c_str(), no_case);
    }

    String operator+(const String& lhs, const String& rhs) { return  String(lhs) += rhs; }

    bool operator==(const String& lhs, const String& rhs) { return lhs.compare(rhs) == 0; }
    bool operator!=(const String& lhs, const String& rhs) { return lhs.compare(rhs) != 0; }
    bool operator< (const String& lhs, const String& rhs) { return lhs.compare(rhs) < 0; }
    bool operator> (const String& lhs, const String& rhs) { return lhs.compare(rhs) > 0; }
    bool operator<=(const String& lhs, const String& rhs) { return (lhs != rhs) ? lhs.compare(rhs) < 0 : true; }
    bool operator>=(const String& lhs, const String& rhs) { return (lhs != rhs) ? lhs.compare(rhs) > 0 : true; }

    std::ostream& operator<<(std::ostream& s, const String& in) { return s << in.c_str(); }

namespace detail {

    void filldata<const void*>::fill(std::ostream* stream, const void* in) {
        filldata<const volatile void*>::fill(stream, in);
    }

    void filldata<const volatile void*>::fill(std::ostream* stream, const volatile void* in) {
        if (in) { *stream << in; }
        else { *stream << "nullptr"; }
    }

    template <typename T>
    String toStreamLit(T t) {
        std::ostream* os = tlssPush();
        os->operator<<(t);
        return tlssPop();
    }
} // namespace detail

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
String toString(const char* in) { return String("\"") + (in ? in : "{null string}") + "\""; }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

#if DOCTEST_MSVC >= DOCTEST_COMPILER(19, 20, 0)
// see this issue on why this is needed: https://github.com/doctest/doctest/issues/183
String toString(const std::string& in) { return in.c_str(); }
#endif // VS 2019

String toString(const String& in) { return in; }

String toString(std::nullptr_t) { return "nullptr"; }

String toString(bool in) { return in ? "true" : "false"; }

String toString(float in) { return toStreamLit(in); }
String toString(double in) { return toStreamLit(in); }
String toString(double long in) { return toStreamLit(in); }

String toString(char in) { return toStreamLit(static_cast<signed>(in)); }
String toString(char signed in) { return toStreamLit(static_cast<signed>(in)); }
String toString(char unsigned in) { return toStreamLit(static_cast<unsigned>(in)); }
String toString(short in) { return toStreamLit(in); }
String toString(short unsigned in) { return toStreamLit(in); }
String toString(signed in) { return toStreamLit(in); }
String toString(unsigned in) { return toStreamLit(in); }
String toString(long in) { return toStreamLit(in); }
String toString(long unsigned in) { return toStreamLit(in); }
String toString(long long in) { return toStreamLit(in); }
String toString(long long unsigned in) { return toStreamLit(in); }

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
