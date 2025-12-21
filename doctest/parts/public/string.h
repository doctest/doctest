namespace doctest {
#ifndef DOCTEST_CONFIG_STRING_SIZE_TYPE
#define DOCTEST_CONFIG_STRING_SIZE_TYPE unsigned
#endif

    // A 24 byte string class (can be as small as 17 for x64 and 13 for x86) that can hold strings with length
    // of up to 23 chars on the stack before going on the heap - the last byte of the buffer is used for:
    // - "is small" bit - the highest bit - if "0" then it is small - otherwise its "1" (128)
    // - if small - capacity left before going on the heap - using the lowest 5 bits
    // - if small - 2 bits are left unused - the second and third highest ones
    // - if small - acts as a null terminator if strlen() is 23 (24 including the null terminator)
    //              and the "is small" bit remains "0" ("as well as the capacity left") so its OK
    // Idea taken from this lecture about the string implementation of facebook/folly - fbstring
    // https://www.youtube.com/watch?v=kPR8h4-qZdk
    // TODO:
    // - optimizations - like not deleting memory unnecessarily in operator= and etc.
    // - resize/reserve/clear
    // - replace
    // - back/front
    // - iterator stuff
    // - find & friends
    // - push_back/pop_back
    // - assign/insert/erase
    // - relational operators as free functions - taking const char* as one of the params
    class DOCTEST_INTERFACE String
    {
    public:
        using size_type = DOCTEST_CONFIG_STRING_SIZE_TYPE;

    private:
        static DOCTEST_CONSTEXPR size_type len  = 24;      //!OCLINT avoid private static members
        static DOCTEST_CONSTEXPR size_type last = len - 1; //!OCLINT avoid private static members

        struct view // len should be more than sizeof(view) - because of the final byte for flags
        {
            char*    ptr;
            size_type size;
            size_type capacity;
        };

        union
        {
            char buf[len]; // NOLINT(*-avoid-c-arrays)
            view data;
        };

        char* allocate(size_type sz);

        bool isOnStack() const noexcept { return (buf[last] & 128) == 0; }
        void setOnHeap() noexcept;
        void setLast(size_type in = last) noexcept;
        void setSize(size_type sz) noexcept;

        void copy(const String& other);

    public:
        static DOCTEST_CONSTEXPR size_type npos = static_cast<size_type>(-1);

        String() noexcept;
        ~String();

        // cppcheck-suppress noExplicitConstructor
        String(const char* in);
        String(const char* in, size_type in_size);

        String(std::istream& in, size_type in_size);

        String(const String& other);
        String& operator=(const String& other);

        String& operator+=(const String& other);

        String(String&& other) noexcept;
        String& operator=(String&& other) noexcept;

        char  operator[](size_type i) const;
        char& operator[](size_type i);

        // the only functions I'm willing to leave in the interface - available for inlining
        const char* c_str() const { return const_cast<String*>(this)->c_str(); } // NOLINT
        char*       c_str() {
            if (isOnStack()) {
                return reinterpret_cast<char*>(buf);
            }
            return data.ptr;
        }

        size_type size() const;
        size_type capacity() const;

        String substr(size_type pos, size_type cnt = npos) &&;
        String substr(size_type pos, size_type cnt = npos) const &;

        size_type find(char ch, size_type pos = 0) const;
        size_type rfind(char ch, size_type pos = npos) const;

        int compare(const char* other, bool no_case = false) const;
        int compare(const String& other, bool no_case = false) const;

        friend DOCTEST_INTERFACE std::ostream& operator<<(std::ostream& s, const String& in);
    };

    DOCTEST_INTERFACE String operator+(const String& lhs, const String& rhs);

    DOCTEST_INTERFACE bool operator==(const String& lhs, const String& rhs);
    DOCTEST_INTERFACE bool operator!=(const String& lhs, const String& rhs);
    DOCTEST_INTERFACE bool operator<(const String& lhs, const String& rhs);
    DOCTEST_INTERFACE bool operator>(const String& lhs, const String& rhs);
    DOCTEST_INTERFACE bool operator<=(const String& lhs, const String& rhs);
    DOCTEST_INTERFACE bool operator>=(const String& lhs, const String& rhs);

namespace detail {

// MSVS 2015 :(
#if !DOCTEST_CLANG && defined(_MSC_VER) && _MSC_VER <= 1900
    template <typename T, typename = void>
    struct has_global_insertion_operator : types::false_type { };

    template <typename T>
    struct has_global_insertion_operator<T, decltype(::operator<<(declval<std::ostream&>(), declval<const T&>()), void())> : types::true_type { };

    template <typename T, typename = void>
    struct has_insertion_operator { static DOCTEST_CONSTEXPR bool value = has_global_insertion_operator<T>::value; };

    template <typename T, bool global>
    struct insert_hack;

    template <typename T>
    struct insert_hack<T, true> {
        static void insert(std::ostream& os, const T& t) { ::operator<<(os, t); }
    };

    template <typename T>
    struct insert_hack<T, false> {
        static void insert(std::ostream& os, const T& t) { operator<<(os, t); }
    };

    template <typename T>
    using insert_hack_t = insert_hack<T, has_global_insertion_operator<T>::value>;
#else
    template <typename T, typename = void>
    struct has_insertion_operator : types::false_type { };
#endif

    template <typename T>
    struct has_insertion_operator<T, decltype(operator<<(declval<std::ostream&>(), declval<const T&>()), void())> : types::true_type { };

    template <typename T>
    struct should_stringify_as_underlying_type {
        static DOCTEST_CONSTEXPR bool value = detail::types::is_enum<T>::value && !doctest::detail::has_insertion_operator<T>::value;
    };

    DOCTEST_INTERFACE std::ostream* tlssPush();
    DOCTEST_INTERFACE String tlssPop();

    template <bool C>
    struct StringMakerBase {
        template <typename T>
        static String convert(const DOCTEST_REF_WRAP(T)) {
#ifdef DOCTEST_CONFIG_REQUIRE_STRINGIFICATION_FOR_ALL_USED_TYPES
            static_assert(deferred_false<T>::value, "No stringification detected for type T. See string conversion manual");
#endif
            return "{?}";
        }
    };

    template <typename T>
    struct filldata;

    template <typename T>
    void filloss(std::ostream* stream, const T& in) {
        filldata<T>::fill(stream, in);
    }

    template <typename T, size_t N>
    void filloss(std::ostream* stream, const T (&in)[N]) { // NOLINT(*-avoid-c-arrays)
        // T[N], T(&)[N], T(&&)[N] have same behaviour.
        // Hence remove reference.
        filloss<typename types::remove_reference<decltype(in)>::type>(stream, in);
    }

    template <typename T>
    String toStream(const T& in) {
        std::ostream* stream = tlssPush();
        filloss(stream, in);
        return tlssPop();
    }

    template <>
    struct StringMakerBase<true> {
        template <typename T>
        static String convert(const DOCTEST_REF_WRAP(T) in) {
            return toStream(in);
        }
    };

} // namespace detail

    template <typename T>
    struct StringMaker : public detail::StringMakerBase<
        detail::has_insertion_operator<T>::value || detail::types::is_pointer<T>::value || detail::types::is_array<T>::value>
    {};

#ifndef DOCTEST_STRINGIFY
#ifdef DOCTEST_CONFIG_DOUBLE_STRINGIFY
#define DOCTEST_STRINGIFY(...) toString(toString(__VA_ARGS__))
#else
#define DOCTEST_STRINGIFY(...) toString(__VA_ARGS__)
#endif
#endif

    template <typename T>
    String toString() {
    #if DOCTEST_CLANG == 0 && DOCTEST_GCC == 0 && DOCTEST_ICC == 0
        String ret = __FUNCSIG__; // class doctest::String __cdecl doctest::toString<TYPE>(void)
        String::size_type beginPos = ret.find('<');
        return ret.substr(beginPos + 1, ret.size() - beginPos - static_cast<String::size_type>(sizeof(">(void)")));
    #else
        String ret = __PRETTY_FUNCTION__; // doctest::String toString() [with T = TYPE]
        String::size_type begin = ret.find('=') + 2;
        return ret.substr(begin, ret.size() - begin - 1);
    #endif // Compiler
    }

    template <typename T, typename detail::types::enable_if<!detail::should_stringify_as_underlying_type<T>::value, bool>::type = true>
    String toString(const DOCTEST_REF_WRAP(T) value) {
        return StringMaker<T>::convert(value);
    }

    inline String&& toString(String&& in) { return static_cast<String&&>(in); }

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    DOCTEST_INTERFACE String toString(const char* in);
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

#if DOCTEST_MSVC >= DOCTEST_COMPILER(19, 20, 0)
    // see this issue on why this is needed: https://github.com/doctest/doctest/issues/183
    DOCTEST_INTERFACE String toString(const std::string& in);
#endif // VS 2019

    DOCTEST_INTERFACE String toString(const String& in);

    DOCTEST_INTERFACE String toString(std::nullptr_t);

    DOCTEST_INTERFACE String toString(bool in);

    DOCTEST_INTERFACE String toString(float in);
    DOCTEST_INTERFACE String toString(double in);
    DOCTEST_INTERFACE String toString(double long in);

    DOCTEST_INTERFACE String toString(char in);
    DOCTEST_INTERFACE String toString(char signed in);
    DOCTEST_INTERFACE String toString(char unsigned in);
    DOCTEST_INTERFACE String toString(short in);
    DOCTEST_INTERFACE String toString(short unsigned in);
    DOCTEST_INTERFACE String toString(signed in);
    DOCTEST_INTERFACE String toString(unsigned in);
    DOCTEST_INTERFACE String toString(long in);
    DOCTEST_INTERFACE String toString(long unsigned in);
    DOCTEST_INTERFACE String toString(long long in);
    DOCTEST_INTERFACE String toString(long long unsigned in);

    template <typename T, typename detail::types::enable_if<detail::should_stringify_as_underlying_type<T>::value, bool>::type = true>
    String toString(const DOCTEST_REF_WRAP(T) value) {
        using UT = typename detail::types::underlying_type<T>::type;
        return (DOCTEST_STRINGIFY(static_cast<UT>(value)));
    }

namespace detail {
    template <typename T>
    struct filldata
    {
        static void fill(std::ostream* stream, const T& in) {
    #if defined(_MSC_VER) && _MSC_VER <= 1900
        insert_hack_t<T>::insert(*stream, in);
    #else
        operator<<(*stream, in);
    #endif // _MSV_VER
        }
    };

    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866)
    // NOLINTBEGIN(*-avoid-c-arrays)
    template <typename T, size_t N>
    struct filldata<T[N]> {
        static void fill(std::ostream* stream, const T(&in)[N]) {
            *stream << "[";
            for (size_t i = 0; i < N; i++) {
                if (i != 0) { *stream << ", "; }
                *stream << (DOCTEST_STRINGIFY(in[i]));
            }
            *stream << "]";
        }
    };
    // NOLINTEND(*-avoid-c-arrays)
    DOCTEST_MSVC_SUPPRESS_WARNING_POP

    // Specialized since we don't want the terminating null byte!
    // NOLINTBEGIN(*-avoid-c-arrays)
    template <size_t N>
    struct filldata<const char[N]> {
        static void fill(std::ostream* stream, const char (&in)[N]) {
            *stream << String(in, in[N - 1] ? N : N - 1);
        } // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
    };
    // NOLINTEND(*-avoid-c-arrays)

    template <>
    struct filldata<const void*> {
        DOCTEST_INTERFACE static void fill(std::ostream* stream, const void* in);
    };

    template <>
    struct filldata<const volatile void*> {
        DOCTEST_INTERFACE static void fill(std::ostream* stream, const volatile void* in);
    };

    template <typename T>
    struct filldata<T*> {
        DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4180)
        static void fill(std::ostream* stream, const T* in) {
        DOCTEST_MSVC_SUPPRESS_WARNING_POP
        DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wmicrosoft-cast")
            filldata<const volatile void*>::fill(stream,
        #if DOCTEST_GCC == 0 || DOCTEST_GCC >= DOCTEST_COMPILER(4, 9, 0)
                reinterpret_cast<const volatile void*>(in)
        #else
                *reinterpret_cast<const volatile void* const*>(&in)
        #endif // DOCTEST_GCC
            );
        DOCTEST_CLANG_SUPPRESS_WARNING_POP
        }
    };

    #ifndef DOCTEST_CONFIG_DISABLE
    template <typename L, typename R>
    String stringifyBinaryExpr(const DOCTEST_REF_WRAP(L) lhs, const char* op,
                               const DOCTEST_REF_WRAP(R) rhs) {
        return (DOCTEST_STRINGIFY(lhs)) + op + (DOCTEST_STRINGIFY(rhs));
    }
    #endif // DOCTEST_CONFIG_DISABLE
} //namespace detail

} // namespace doctest
