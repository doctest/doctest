#pragma once

// if registering is not disabled
#if !defined(DOCTEST_GLOBAL_DISABLE)

// if this header is included into the main doctest.h header the functions
// defined here should be marked as inline so compilation doesn't fail
#define DOCTEST_INLINE inline
#ifdef DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#undef DOCTEST_INLINE
#define DOCTEST_INLINE
#endif // DOCTEST_DONT_INCLUDE_IMPLEMENTATION

// MSVC fix to not complain about using strcpy instead of strcpy_s
#ifdef _MSC_VER
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

// required includes
#include <map>
#include <cstring> // C string manipulation
#include <cstdlib> // malloc
#include <cstdio>  // printf

// TODO: REMOVE! only while dev!
#include <iostream>
using namespace std;

// the namespace used by all functions generated/registered by this library
namespace doctestns
{

// the function type this library works with
typedef void (*functionType)(void);

// a struct defining a registered test callback
struct FunctionSignature
{
    unsigned line;
    const char* file;
    const char* method;
    const char* suite;

    const char* name; // not used for comparing

    bool operator<(const FunctionSignature& other) const
    {
        if(line != other.line)
            return line < other.line;
        int res = strcmp(file, other.file);
        if(res != 0)
            return res < 0;
        res = strcmp(suite, other.suite);
        if(res != 0)
            return res < 0;
        return strcmp(method, other.method) < 0;
    }
};

// std::allocator compatible using plain malloc to avoid operator new
template <typename T>
struct MallocAllocator
{
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    pointer allocate(size_type n, const void* = 0)
    {
        return static_cast<pointer>(malloc(sizeof(value_type) * n));
    }

    void deallocate(pointer ptr, size_type)
    {
        free(ptr);
    }

    // boilerplate follows
    MallocAllocator()
    {
    }

    MallocAllocator(const MallocAllocator&)
    {
    }

    template <typename Other>
    MallocAllocator(const MallocAllocator<Other>&)
    {
    }

    MallocAllocator& operator=(const MallocAllocator&)
    {
        return *this;
    }

    template <class Other>
    MallocAllocator& operator=(const MallocAllocator<Other>&)
    {
        return *this;
    }

    template <typename Other>
    struct rebind
    {
        typedef MallocAllocator<Other> other;
    };

    size_type max_size() const throw()
    {
        // ugly size_t maximum but don't want to drag <numeric_limits> just because of this
        return std::size_t(-1) / sizeof(T);
    }

    pointer address(reference ref) const
    {
        return &ref;
    }

    const_pointer address(const_reference ref) const
    {
        return &ref;
    }

    void construct(pointer ptr, const value_type& val)
    {
        ::new (ptr) value_type(val);
    }

    void destroy(pointer ptr)
    {
        ptr->~value_type();
    }
};

template <typename T, typename U>
inline bool operator==(const MallocAllocator<T>&, const MallocAllocator<U>&)
{
    return true;
}

template <typename T, typename U>
inline bool operator!=(const MallocAllocator<T>& a, const MallocAllocator<U>& b)
{
    return !(a == b);
}

// typedef for the map with the custom allocator
typedef std::map<FunctionSignature, functionType, std::less<FunctionSignature>,
                 MallocAllocator<std::pair<const FunctionSignature, functionType> > > mapType;

// matching of a string against a wildcard mask (case sensitivity configurable)
// taken from http://www.emoticode.net/c/simple-wildcard-string-compare-globbing-function.html
DOCTEST_INLINE int wildcmp(const char* str, const char* wild)
{
    const char* cp = 0, * mp = 0;

    while((*str) && (*wild != '*')) {
        if(
#ifdef DOCTEST_CASE_SENSITIVE
            (*wild != *str)
#else  // DOCTEST_CASE_SENSITIVE
            // rolled my own tolower() to not include more headers
            ((*wild >= 'A' && *wild <= 'Z') ? *wild + 32 : *wild) != ((*str >= 'A' && *str <= 'Z') ? *str + 32 : *str)
#endif // DOCTEST_CASE_SENSITIVE
            && (*wild != '?')) {
            return 0;
        }
        wild++;
        str++;
    }

    while(*str) {
        if(*wild == '*') {
            if(!*++wild) {
                return 1;
            }
            mp = wild;
            cp = str + 1;
        } else if(
#ifdef DOCTEST_CASE_SENSITIVE
            (*wild == *str)
#else  // DOCTEST_CASE_SENSITIVE
            // rolled my own tolower() to not include more headers
            ((*wild >= 'A' && *wild <= 'Z') ? *wild + 32 : *wild) == ((*str >= 'A' && *str <= 'Z') ? *str + 32 : *str)
#endif // DOCTEST_CASE_SENSITIVE
            || (*wild == '?')) {
            wild++;
            str++;
        } else {
            wild = mp;
            str = cp++;
        }
    }

    while(*wild == '*') {
        wild++;
    }
    return !*wild;
}

// trick to register a global variable in a header - as a static var in an inline method
DOCTEST_INLINE mapType& getRegisteredFunctions()
{
    static mapType value;
    return value;
}

// trick to register a global variable in a header - as a static var in an inline method
DOCTEST_INLINE const char*& getTestSuiteName()
{
    static const char* value = "";
    return value;
}

// sets the current test suite
DOCTEST_INLINE int setTestSuiteName(const char* name)
{
    getTestSuiteName() = name;
    return 0;
}

// used by the macros for registering doctest callbacks (short name for small codegen)
DOCTEST_INLINE int registerFunction(functionType f, unsigned line, const char* file,
                                    const char* method, const char* name)
{
    mapType& registeredFunctions = getRegisteredFunctions();
    // initialize the record
    FunctionSignature signature;
    signature.line = line;
    signature.file = file;
    signature.suite = getTestSuiteName();
    signature.method = method;
    signature.name = name;
    // insert the record
    registeredFunctions.insert(std::pair<FunctionSignature, functionType>(signature, f));
    return 0;
}

// parses a comma separated list of words after a pattern in one of the arguments in argv
DOCTEST_INLINE void parseArgs(int argc, char** argv, const char* pattern, char**& filters,
                              size_t& filterCount)
{
    char* filtersString = 0;
    for(int i = 0; i < argc; ++i) {
        const char* temp = strstr(argv[i], pattern);
        if(temp) {
            temp += strlen(pattern);
            size_t len = strlen(temp);
            if(len) {
                filtersString = static_cast<char*>(malloc(len + 1));
                strcpy(filtersString, temp);
                break;
            }
        }
    }

    // if we have found the filter string
    if(filtersString) {
        const size_t maxFiltersInList = 1024; // ought to be enough
        filters = static_cast<char**>(malloc(sizeof(char*) * maxFiltersInList));
        // tokenize with "," as a separator for the first maxFiltersInList filters
        char* pch = strtok(filtersString, ",");
        while(pch != 0) {
            size_t len = strlen(pch);
            if(len && filterCount < maxFiltersInList) {
                filters[filterCount] = static_cast<char*>(malloc(len + 1));
                strcpy(filters[filterCount], pch);
                ++filterCount;
            }
            pch = strtok(0, ",");
        }
        free(filtersString);
    }
}

// checks if the name matches any of the filters (and can be configured what to do when empty)
DOCTEST_INLINE int matchesAny(const char* name, char** filters, size_t filterCount, bool matchEmpty)
{
    if(filterCount == 0 && matchEmpty)
        return true;
    for(size_t i = 0; i < filterCount; ++i) {
        if(wildcmp(name, filters[i])) {
            return true;
        }
    }
    return false;
}

DOCTEST_INLINE void invokeAllFunctions(int argc, char** argv)
{
    char** filters[6] = {0, 0, 0, 0, 0, 0};
    size_t counts[6] = {0, 0, 0, 0, 0, 0};
    parseArgs(argc, argv, "-doctest_file=", filters[0], counts[0]);
    parseArgs(argc, argv, "-doctest_file_exclude=", filters[1], counts[1]);
    parseArgs(argc, argv, "-doctest_suite=", filters[2], counts[2]);
    parseArgs(argc, argv, "-doctest_suite_exclude=", filters[3], counts[3]);
    parseArgs(argc, argv, "-doctest_name=", filters[4], counts[4]);
    parseArgs(argc, argv, "-doctest_name_exclude=", filters[5], counts[5]);

    // invoke the registered functions
    mapType& registeredFunctions = getRegisteredFunctions();
    mapType::iterator it;
    for(it = registeredFunctions.begin(); it != registeredFunctions.end(); ++it) {
        if(!matchesAny(it->first.file, filters[0], counts[0], true))
            continue;
        if(matchesAny(it->first.file, filters[1], counts[1], false))
            continue;
        if(!matchesAny(it->first.suite, filters[2], counts[2], true))
            continue;
        if(matchesAny(it->first.suite, filters[3], counts[3], false))
            continue;
        if(!matchesAny(it->first.name, filters[4], counts[4], true))
            continue;
        if(matchesAny(it->first.name, filters[5], counts[5], false))
            continue;

        // call the function if it passes all the filtering
        try {
            it->second();
        } catch(std::exception& e) {
            printf("%s\n", e.what());
        } catch(...) {
            printf("Unknown exception caught!\n");
        }
    }

    // cleanup buffers
    for(size_t i = 0; i < 6; i++) {
        if(counts[i] > 0) {
            for(size_t k = 0; k < counts[i]; ++k)
                free(filters[i][k]);
            free(filters[i]);
        }
    }
}
} // namespace doctestns

// undef stuff defined at the top to keep things isolated
#ifdef UNDEFINE_CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#undef UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // UNDEFINE_CRT_SECURE_NO_WARNINGS

#undef DOCTEST_INLINE

#endif // DOCTEST_GLOBAL_DISABLE
