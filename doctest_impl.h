#pragma once

// if registering is not disabled
#if !defined DOCTEST_GLOBAL_DISABLE

// if this header is included into the main doctest.h header the functions
// defined here should be marked as inline so compilation doesn't fail
#define DOCTEST_INLINE inline
#if defined DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#undef DOCTEST_INLINE
#define DOCTEST_INLINE
#endif

// MSVC fix to not complain about using strcpy instead of strcpy_s
#if defined _MSC_VER
#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif
#endif

// required includes
#include <map>
#include <cstring> // C string manipulation
#include <cstdlib> // malloc

// the namespace used by all functions generated/registered by this library
namespace doctestns {

// the function type this library works with
typedef void (*functionType)(void);

// a struct defining a registered test callback
struct functionSignature
{
    unsigned line;
    const char* file;
    const char* method;

    const char* name; // not used for comparing

    bool operator<(const functionSignature& other) const {
        if(line != other.line)
            return line < other.line;
        int fileCmp = strcmp(file, other.file);
        if(fileCmp != 0)
            return fileCmp < 0;
        return strcmp(method, other.method) < 0;
    }
};

template<typename T>
struct MallocAllocator
{
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    pointer allocate(size_type n, const void* = 0) {
        return static_cast<pointer>(malloc(sizeof(value_type) * n));
    }

    void deallocate(pointer ptr, size_type) {
        free(ptr);
    }

    // boilerplate follows
    MallocAllocator() {}

    MallocAllocator(const MallocAllocator&) {}

    template <typename Other>
    MallocAllocator(const MallocAllocator<Other>&) {}

    MallocAllocator& operator=(const MallocAllocator&) { return *this; }

    template <class Other>
    MallocAllocator& operator=(const MallocAllocator<Other>&) { return *this; }

    template <typename Other>
    struct rebind { typedef MallocAllocator<Other> other; };

    pointer address(reference ref) const {
        return &ref;
    }

    const_pointer address(const_reference ref) const {
        return &ref;
    }

    void construct(pointer ptr, const value_type& val) {
        ::new(ptr) value_type(val);
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }
};

template <typename T, typename U>
inline bool operator==(const MallocAllocator<T>&, const MallocAllocator<U>&) {
    return true;
}

template <typename T, typename U>
inline bool operator!=(const MallocAllocator<T>& a, const MallocAllocator<U>& b) {
    return !(a == b);
}

typedef std::map<functionSignature, functionType, std::less<functionSignature>,
    MallocAllocator<std::pair<const functionSignature, functionType> > > mapType;

// trick to register a global variable in a header - as a static var in an inline method
DOCTEST_INLINE mapType& getRegisteredFunctions() {
    static mapType value;
    return value;
}

// used by the macros for registering doctest callbacks (short name for small codegen)
DOCTEST_INLINE int r(functionType f, unsigned line, const char* file, const char* method,
                     const char* name) {
    mapType& registeredFunctions = getRegisteredFunctions();
    // initialize the record
    functionSignature signature;
    signature.line = line;
    signature.file = file;
    signature.method = method;
    signature.name = name;
    // insert the record
    registeredFunctions.insert(std::pair<functionSignature, functionType>(signature, f));
    return 0;
}

DOCTEST_INLINE void invokeAllFunctions(int argc, char** argv) {
    mapType& registeredFunctions = getRegisteredFunctions();
    // if atleast one test has been registered
    if(registeredFunctions.size() > 0) {
        char* filtersString = 0;
        char** filters = 0;
        size_t filterCount = 0;
        // locate the filters string from the arguments (with comma separated filters)
        for(int i = 1; i < argc; ++i) {
            const char* temp = strstr(argv[i], "-doctest=");
            if(temp) {
                temp += strlen("-doctest=");
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
        }
        // invoke the registered functions
        mapType::iterator it;
        for(it = registeredFunctions.begin(); it != registeredFunctions.end(); ++it) {
            // if there are any filters given
            if(filterCount) {
                // call the callback if atleast one filter matches
                for(size_t i = 0; i < filterCount; ++i) {
                    if(strstr(it->first.name, filters[i])) {
                        it->second();
                        break;
                    }
                }
            } else {
                // just invoke each test if there are no filters
                it->second();
            }
        }
        // cleanup buffers
        if(filters) {
            for(size_t i = 0; i < filterCount; ++i)
                free(filters[i]);
            free(filters);
            free(filtersString);
        }
    }
}
} // namespace doctestns

// undef stuff defined at the top to keep things isolated
#if defined UNDEFINE_CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#undef UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif

#undef DOCTEST_INLINE

#endif // DOCTEST_GLOBAL_DISABLE
