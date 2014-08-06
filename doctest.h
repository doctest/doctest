#pragma once
#ifndef __DOCTEST_HEADER_INCLUDED__
#define __DOCTEST_HEADER_INCLUDED__

// == BENCHMARK:
//  10k doctests in 1 cpp == 32 sec build time on my machine
//   5k doctests in 1 cpp ==  9 sec build time on my machine
// 2.5k doctests in 1 cpp ==  3 sec build time on my machine
// == TODO:
// - disabling function registration with a macro
// - think about in-class doctests
// - macro for making the lib not header only - for multi dll scenarios
//   (will force the user to use a macro in only 1 dll in only 1 source file)
//   (another option is to include the same header in only one source file
//    having defined something)
// - asserts
// - reporting
// - documentation

#if !defined(DOCTEST_NOT_HEADER_ONLY)
#include <map>
#include <cstring>
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
        if(fileCmp != 0) return fileCmp;
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
                    filtersString = (char*)malloc(len + 1);
                    strcpy(filtersString, temp);
                    break;
                }
            }
        }
        // if we have found the filter string
        if(filtersString) {
            filters = (char**)malloc(1000);
            //printf("%s\n", filtersString);
            char* pch = strtok(filtersString, ",");
            while(pch != 0) {
                unsigned len = strlen(pch);
                if(len) {
                    filters[filterCount] = (char*)malloc(len + 1);
                    strcpy(filters[filterCount], pch);
                    ++filterCount;
                }
                pch = strtok(NULL, ",");
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
                free(filters[i]);
            free(filters);
            free(filtersString);
        }
    }
}
#else // DOCTEST_NOT_HEADER_ONLY
int r(functionType f, unsigned line, const char* file, const char* method, const char* name);
void invokeAllFunctions(int argc, char** argv);
#endif // DOCTEST_NOT_HEADER_ONLY

// ============================================================================
// internal macros
// ============================================================================

#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#if defined(__COUNTER__) // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif

// ============================================================================
// macros for creating and registering test functions - produce optimal codegen
// ============================================================================

#define DOCTEST_REGISTER_FUNCTION(f, name) \
static int DOCTEST_ANONYMOUS_NAME(a)=r(f,__LINE__,__FILE__,"",#name);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, m) \
namespace doctestns{static int DOCTEST_ANONYMOUS_NAME(a)=r(&x::m,0,__FILE__,#m,"");}

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) \
namespace doctestns{void f();DOCTEST_REGISTER_FUNCTION(f, name)}inline void doctestns::f()

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name) \
namespace doctestns{struct der:base{void f();};inline void func(){der v;v.f();}\
static int DOCTEST_ANONYMOUS_NAME(a)=r(func,__LINE__,__FILE__,"",#name);}\
inline void doctestns::der::f()

// for registering normal doctests
#define doctest(name) \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), name)

// for registering doctests with a fixture
#define doctest_fixture(x, name) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f), name)

} // namespace doctestns

#endif // __DOCTEST_HEADER_INCLUDED__
