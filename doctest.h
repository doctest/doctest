#pragma once
#ifndef __DOCTEST_HEADER_INCLUDED__
#define __DOCTEST_HEADER_INCLUDED__

// An alternative to including these is to put the functionality that registers
// functions into the map into a macro and require the user to use that macro
// once somewhere in a source file and to have these includes there before it.
#include <map>
#include <cstring>
#include <cstdio>

// ============================================================================
// ================================== BEGIN ===================================
// ============================================================================

namespace doctestns {

// call the registered tests with this
#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(c, v) doctestns::invokeAllFunctions(c, v);

typedef void (*functionType)(void);

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

// trick to register a global variable in a header - as a static var in an inline method
inline std::map<functionSignature, functionType>*& getRegisteredFunctions() {
    static std::map<functionSignature, functionType>* value = 0;
    return value;
}

inline int r(functionType f, unsigned line, const char* file, const char* method, const char* name) {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    if(!registeredFunctions)
        registeredFunctions = new std::map<functionSignature, functionType>;
    functionSignature signature;
    signature.line = line;
    signature.file = file;
    signature.method = method;
    signature.name = name;
    registeredFunctions->insert(std::pair<functionSignature, functionType>(signature, f));
    return 0;
}

inline void invokeAllFunctions(int argc, char** argv) {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    if(registeredFunctions)
        for(std::map<functionSignature, functionType>::iterator it = registeredFunctions->begin();
            it != registeredFunctions->end(); ++it) {
            
            it->second();
        }
}

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

#define DOCTEST_REGISTER_FUNCTION(f, x) \
static int DOCTEST_ANONYMOUS_NAME(a)=r(f,__LINE__,__FILE__,"",#x);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, m) \
namespace doctestns{static int DOCTEST_ANONYMOUS_NAME(a)=r(&x::m,0,__FILE__,#m,"");}

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) \
namespace doctestns{void f();DOCTEST_REGISTER_FUNCTION(f, name)}inline void doctestns::f()

#define DOCTEST_IMPLEMENT_FIXTURE(name, base, func) \
namespace doctestns{struct name:base{void f();};inline void func(){name v;v.f();}static int DOCTEST_ANONYMOUS_NAME(a)=r(func,__LINE__,__FILE__,"","");}inline void doctestns::name::f()

// for registering normal doctests
#define doctest(name) \
DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f), name)

// for registering doctests with a fixture
#define doctest_fixture(x, y) \
DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS_NAME(F), x, DOCTEST_ANONYMOUS_NAME(f))

} // namespace doctestns

// ============================================================================
// ================================== END =====================================
// ============================================================================

#endif // __DOCTEST_HEADER_INCLUDED__
