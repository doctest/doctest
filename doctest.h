#pragma once
#ifndef __DOCTEST_HEADER_INCLUDED__
#define __DOCTEST_HEADER_INCLUDED__

// An alternative to including these is to put the functionality that registers
// functions into the map into a macro and require the user to use that macro
// once somewhere in a source file and to have these includes there before it.
#include <map>
#include <cstring>

// ============================================================================
// ================================== BEGIN ===================================
// ============================================================================

namespace doctestns {

// call the registered tests with this
#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS() doctestns::invokeAllFunctions();

typedef void (*functionType)(void);

struct functionSignature {
    unsigned line;
    const char* file;
    
    bool operator<(const functionSignature& other) const {
        if(line != other.line) return line < other.line;
        return strcmp(file, other.file) < 0;
    }
};

inline std::map<functionSignature, functionType>*& getRegisteredFunctions() {
    static std::map<functionSignature, functionType>* value = 0;
    return value;
}

inline int registerFunctionInGlobalArray(functionType f, unsigned line, const char* file) {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    if(!registeredFunctions)
        registeredFunctions = new std::map<functionSignature, functionType>;
    functionSignature signature;
    signature.line = line;
    signature.file = file;
    registeredFunctions->insert(std::pair<functionSignature, functionType>(signature, f));
    return 0;
}

inline void invokeAllFunctions() {
    std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
    if(registeredFunctions)
        for(std::map<functionSignature, functionType>::iterator it = registeredFunctions->begin();
            it != registeredFunctions->end(); ++it)
            it->second();
}

// ============================================================================
// invocation macro - executes all the registered test functions
// ============================================================================


// ============================================================================
// internal macros
// ============================================================================

#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#if defined(__COUNTER__)
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif

// ============================================================================
// macros for creating and registering test functions
// ============================================================================

#define DOCTEST_REGISTER_FUNCTION(f)                                           \
    static int DOCTEST_ANONYMOUS_NAME(a) =                                     \
        doctestns::registerFunctionInGlobalArray(f, __LINE__, __FILE__);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, f)                                  \
    static int DOCTEST_ANONYMOUS_NAME(x##f) =                                  \
        doctestns::registerFunctionInGlobalArray(&x::f, __LINE__, __FILE__);

#define DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(x)                               \
    DOCTEST_REGISTER_CLASS_FUNCTION(x, check)

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(x)                                \
    namespace doctestns {                                                      \
        void x();                                                              \
        DOCTEST_REGISTER_FUNCTION(x)                                           \
    }                                                                          \
    inline void doctestns::x()                                                 \

#define doctest DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f))

#define DOCTEST_IMPLEMENT_FIXTURE(name, base, func)                            \
    namespace doctestns {                                                      \
        struct name : base {                                                   \
            inline void f();                                                   \
        };                                                                     \
        inline void func() {                                                   \
            name var;                                                          \
            var.f();                                                           \
        }                                                                      \
        static int DOCTEST_ANONYMOUS_NAME(a) =                                 \
            doctestns::registerFunctionInGlobalArray(func, __LINE__, __FILE__);\
    }                                                                          \
    inline void doctestns::name::f()

#define doctest_fixture(x) DOCTEST_IMPLEMENT_FIXTURE(                          \
    DOCTEST_ANONYMOUS_NAME(x), x, DOCTEST_ANONYMOUS_NAME(f))

} // namespace doctestns

// ============================================================================
// ================================== END =====================================
// ============================================================================

#endif // __DOCTEST_HEADER_INCLUDED__