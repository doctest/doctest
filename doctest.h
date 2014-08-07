#pragma once
#ifndef __DOCTEST_HEADER_INCLUDED__
#define __DOCTEST_HEADER_INCLUDED__

//#define DOCTEST_GLOBAL_DISABLE

// == BENCHMARK:
//  10k doctests in 1 cpp == 32 sec build time on my machine
//   5k doctests in 1 cpp ==  9 sec build time on my machine
// 2.5k doctests in 1 cpp ==  3 sec build time on my machine
// == TODO:
// - think about not defining _CRT_SECURE_NO_WARNINGS 
// - think about not doing warning(disable: 4003) (separate set of macros for named tests?)
// - think about in-class doctests
// - macro for making the lib not header only - for multi dll scenarios
//   (will force the user to use a macro in only 1 dll in only 1 source file)
//   (another option is to include the same header in only one source file
//    having defined something)
// - asserts
// - reporting
// - documentation

#if !defined(DOCTEST_GLOBAL_DISABLE)

#if !defined(DOCTEST_NOT_HEADER_ONLY)

// MSVC fixes
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable: 4003) // not enough actual parameters for macro
#endif // _MSC_VER

// required includes
#include <map>
#include <cstring>
#include <cstdlib> // for atexit
#endif // DOCTEST_NOT_HEADER_ONLY)

namespace doctestns {

// call the registered tests with this
#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv) \
doctestns::invokeAllFunctions(argc, argv);

typedef void (*functionType)(void);

#if !defined(DOCTEST_NOT_HEADER_ONLY)
struct functionSignature {
    unsigned line;
    const char* file;
    const char* method;
    
    const char* name; // not used for comparing
    
    bool operator<(const functionSignature& other) const {
        if(line != other.line) return line < other.line;
        int fileCmp = strcmp(file, other.file);
        if(fileCmp != 0) return fileCmp < 0;
        return strcmp(method, other.method) < 0;
    }
};

void cleanupRegisteredFunctions();

// trick to register a global variable in a header - as a static var in an inline method
inline std::map<functionSignature, functionType>*& getRegisteredFunctions() {
    static std::map<functionSignature, functionType>* value = 0;
    // register the cleanup callback only for the first time this getter is called
    if(value == 0)
        atexit(cleanupRegisteredFunctions);
    return value;
}

inline void cleanupRegisteredFunctions() {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    delete registeredFunctions;
    registeredFunctions = 0;
}

inline int r(functionType f, unsigned line, const char* file, const char* method, const char* name) {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    // if being the first one to register a callback, initialize the storage
    if(!registeredFunctions)
        registeredFunctions = new std::map<functionSignature, functionType>;
    functionSignature signature;
    signature.line = line;
    signature.file = file;
    signature.method = method;
    signature.name = name;
    // insert the record
    registeredFunctions->insert(std::pair<functionSignature, functionType>(signature, f));
    return 0;
}

inline void invokeAllFunctions(int argc, char** argv) {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    // if atleast one test has been registered
    if(registeredFunctions) {
        char* filtersString = 0;
        char** filters = 0;
        unsigned filterCount = 0;
        // locate the filters string from the arguments (with comma separated filters)
        for(int i = 1; i < argc; ++i) {
            const char* temp = strstr(argv[i], "-doctest=");
            if(temp) {
                temp += strlen("-doctest=");
                unsigned len = strlen(temp);
                if(len) {
                    filtersString = new char[len + 1];
                    memcpy(filtersString, temp, len + 1);
                    break;
                }
            }
        }
        // if we have found the filter string
        if(filtersString) {
            const unsigned maxFiltersInCommaSeparatedList = 1024; // ought to be enough
            filters = new char*[maxFiltersInCommaSeparatedList];
            // tokenize with "," as a separator for the first maxFiltersInCommaSeparatedList filters
            char* pch = strtok(filtersString, ",");
            while(pch != 0) {
                unsigned len = strlen(pch);
                if(len && filterCount < maxFiltersInCommaSeparatedList) {
                    filters[filterCount] = new char[len + 1];
                    memcpy(filters[filterCount], pch, len + 1);
                    ++filterCount;
                }
                pch = strtok(0, ",");
            }
        }
        // invoke the registered functions
        for(std::map<functionSignature, functionType>::iterator it = registeredFunctions->begin();
            it != registeredFunctions->end(); ++it) {
            // if there are any filters given
            if(filterCount) {
                // call the callback if atleast one filter matches
                for(unsigned i = 0; i < filterCount; ++i) {
                    if(strstr(it->first.name, filters[i])) {
                        it->second();
                        break;
                    }
                }
            } else {
                it->second();
            }
        }
        // cleanup buffers
        if(filters) {
            for(unsigned i = 0; i < filterCount; ++i)
                delete [] filters[i];
            delete [] filters;
            delete [] filtersString;
        }
    }
}
#else // DOCTEST_NOT_HEADER_ONLY
int r(functionType f, unsigned line, const char* file, const char* method, const char* name);
void invokeAllFunctions(int argc, char** argv);
#endif // DOCTEST_NOT_HEADER_ONLY

} // namespace doctestns

// ============================================================================
// macros for creating and registering test functions - produce optimal codegen
// ============================================================================

// internal macros
#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#if defined(__COUNTER__) // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif

// internal registering macros
#define DOCTEST_REGISTER_FUNCTION(f, name) \
static int DOCTEST_ANONYMOUS_NAME(a)=r(f,__LINE__,__FILE__,"",#name);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, m) \
namespace doctestns{static int DOCTEST_ANONYMOUS_NAME(a)=r(&x::m,0,__FILE__,#m,"");}

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name) \
namespace doctestns{struct der:base{void f();};inline void func(){der v;v.f();}\
static int DOCTEST_ANONYMOUS_NAME(a)=r(func,__LINE__,__FILE__,"",#name);}\
inline void doctestns::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) \
namespace doctestns{void f();DOCTEST_REGISTER_FUNCTION(f, name)}inline void doctestns::f()

// for registering normal doctests
#define doctest(name) \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), name)

// for registering doctests with a fixture
#define doctest_fixture(x, name) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f), name)

// =============================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!  ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_GLOBAL_DISABLE GLOBALLY!        ==
// =============================================================================
#else // DOCTEST_GLOBAL_DISABLE

#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#if defined(__COUNTER__) // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif

#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv)
#define DOCTEST_REGISTER_FUNCTION(f, name)
#define DOCTEST_REGISTER_CLASS_FUNCTION(x, m)
#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name) \
namespace doctestns{struct der:base{void f();};inline void func(){der v;v.f();}}\
inline void doctestns::der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) \
namespace doctestns{void f();DOCTEST_REGISTER_FUNCTION(f, name)}inline void doctestns::f()

// for registering normal doctests
#define doctest(name) \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), name)

// for registering doctests with a fixture
#define doctest_fixture(x, name) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f), name)

#endif // DOCTEST_GLOBAL_DISABLE

#endif // __DOCTEST_HEADER_INCLUDED__
