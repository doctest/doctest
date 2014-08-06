#pragma once

#include <iostream>
#include <vector>
#include <map>
using namespace std;

namespace doctestns {

// ============================================================================
// type of functions used by the test suite
// ============================================================================

typedef void (*functionType)(void);

struct functionSignature {
    unsigned line;
    const char* file;
};

inline std::map<functionSignature, functionType>*& getRegisteredFunctions2() {
    static std::map<functionSignature, functionType>* value = 0;
    return value;
}

inline std::vector<functionType>*& getRegisteredFunctions() {
    static std::vector<functionType>* value = 0;
    return value;
}

inline void registerFunctionInGlobalArray(functionType f) {
    std::vector<functionType>*& registeredFunctions = getRegisteredFunctions();
    if(!registeredFunctions)
        registeredFunctions = new std::vector<functionType>;
    registeredFunctions->push_back(f);
}
inline void invokeAllFunctions() {
    std::vector<functionType>*& registeredFunctions = getRegisteredFunctions();
    if(registeredFunctions)
        for(unsigned i = 0; i < registeredFunctions->size(); ++i)
            registeredFunctions->at(i)();
}

// ============================================================================
// invocation macro - executes all the registered test functions
// ============================================================================

#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS() doctestns::invokeAllFunctions();

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
// getters of global variables
// ============================================================================

template <functionType fn>
int& getGlobalIntForFunction() {
    static int value = 0;
    return value;
}

template <typename T>
int& getGlobalIntForType() {
    static int value = 0;
    return value;
}

template <typename T, functionType fn>
int& getGlobalIntForMember() {
    static int value = 0;
    return value;
}

// ============================================================================
// forward declarations of statics and implementation
// ============================================================================

//extern std::vector<functionType>* registeredFunctions;
void registerFunctionInGlobalArray(functionType);

// ============================================================================
// templated registering functions
// ============================================================================

template <typename T, functionType fn>
int registerMemberFunction(functionType f) {
    int& inited = getGlobalIntForMember<T, fn>();
    if(!inited) registerFunctionInGlobalArray(f);
    return inited = 1;
}

template <functionType fn>
int registerFunction(functionType f) {
    int& inited = getGlobalIntForFunction<fn>();
    if(!inited) registerFunctionInGlobalArray(f);
    return inited = 1;
}

// ============================================================================
// macros for creating and registering test functions
// ============================================================================

#define DOCTEST_REGISTER_FUNCTION(f)                                           \
    static int DOCTEST_ANONYMOUS_NAME(a) = doctestns::registerFunction<&f>(f);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, f)                                  \
    static int DOCTEST_ANONYMOUS_NAME(x##f) =                                  \
        doctestns::registerMemberFunction<x, &x::f>(&x::f);

#define DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(x)                               \
    DOCTEST_REGISTER_CLASS_FUNCTION(x, check)

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(x)                                \
    namespace doctestns {                                                      \
        void x();                                                              \
        DOCTEST_REGISTER_FUNCTION(x)                                           \
    }                                                                          \
    inline void doctestns::x()                                                 \

#define doctest DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f))

#define DOCTEST_INHERIT_FIXTURE(name, base, func)                              \
    namespace doctestns {                                                      \
        struct name : base {                                                   \
            inline void f();                                                   \
        };                                                                     \
        inline void func() {                                                   \
            name var;                                                          \
            var.f();                                                           \
        }                                                                      \
        static int DOCTEST_ANONYMOUS_NAME(a) =                                 \
            doctestns::registerFunction<&func>(func);                          \
    }                                                                          \
    inline void doctestns::name::f()

#define doctest_fixture(x) \
    DOCTEST_INHERIT_FIXTURE(DOCTEST_ANONYMOUS_NAME(x), x, DOCTEST_ANONYMOUS_NAME(f))

} // namespace doctestns

// ============================================================================
// ================================== END =====================================
// ============================================================================

struct MILF {};