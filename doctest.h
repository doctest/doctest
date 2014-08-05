#pragma once

namespace doctestns {

// ============================================================================
// implementation macro - use it in one of your source files for proper linking
// ============================================================================

#define DOCTEST_IMPLEMENT_GLOBALS()                                            \
namespace doctestns {                                                            \
    std::vector<functionType>* registeredFunctions = 0;                     \
    void registerFunctionInGlobalArray(functionType f) {                       \
        if(!registeredFunctions)                                               \
            registeredFunctions = new std::vector<functionType>;               \
        registeredFunctions->push_back(f);                                     \
    }                                                                          \
    void invokeAllFunctions() {                                                \
        if(registeredFunctions)                                                \
            for(unsigned i = 0; i < registeredFunctions->size(); ++i)            \
                registeredFunctions->at(i)();                                  \
    }                                                                          \
} // namespace doctestns

// ============================================================================
// invocation macro - executes all the registered test functions
// ============================================================================

#define DOCTEST_INVOKE_ALL_TEST_FUNCTIONS() doctestns::invokeAllFunctions();

// ============================================================================
// internal macros
// ============================================================================

#define DOCTEST_STRING_CONCATENATE_IMPL(s1, s2) s1##s2
#define DOCTEST_STRING_CONCATENATE(s1, s2)                                     \
    DOCTEST_STRING_CONCATENATE_IMPL(s1, s2)
#define DOCTEST_ANONYMOUS_NAME(x) DOCTEST_STRING_CONCATENATE(x, __LINE__)

// ============================================================================
// type of functions used by the test suite
// ============================================================================

typedef void (*functionType)(void);

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
    static int DOCTEST_ANONYMOUS_NAME(f) = doctestns::registerFunction<&f>(f);

#define DOCTEST_REGISTER_CLASS_FUNCTION(x, f)                                  \
    static int DOCTEST_ANONYMOUS_NAME(x##f) =                                  \
        doctestns::registerMemberFunction<x, &x::f>(&x::f);

#define DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(x)                               \
    DOCTEST_REGISTER_CLASS_FUNCTION(x, check)

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(x)                                \
    void x();                                                                  \
    DOCTEST_REGISTER_FUNCTION(x);                                              \
    inline void x()

#define doctest                                                                \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS_NAME(f))

#define DOCTEST_INHERIT_FIXTURE(name, base, func) \
	struct name : base { \
		inline void f(); \
	}; \
	inline void func() { \
		name var; \
		var.f(); \
	} \
    static int DOCTEST_ANONYMOUS_NAME(func) = doctestns::registerFunction<&func>(func); \
	inline void name::f()

#define doctest_fixture(x) \
    DOCTEST_INHERIT_FIXTURE(DOCTEST_ANONYMOUS_NAME(x), x, DOCTEST_ANONYMOUS_NAME(func))
	
} // namespace doctestns

// ============================================================================
// ================================== END =====================================
// ============================================================================




struct MILF {
	MILF() : a(5) {}
	~MILF() {}
	void mthd() {}
	int a;
};

doctest_fixture(MILF) {
	a = 666;
	mthd();
}

doctest_fixture(MILF) {
	mthd();
	a = 88;
}



struct X {
    static void check() { int a = 5; }

    static void opsa() { int a = 5; }

    static void opsa2() { int a = 5; }
};

DOCTEST_REGISTER_CLASS_CHECK_FUNCTION(X);
DOCTEST_REGISTER_CLASS_FUNCTION(X, opsa);
DOCTEST_REGISTER_CLASS_FUNCTION(X, opsa2);

inline void test() { int a = 5; }

DOCTEST_REGISTER_FUNCTION(test);
DOCTEST_REGISTER_FUNCTION(test);
DOCTEST_REGISTER_FUNCTION(test);
DOCTEST_REGISTER_FUNCTION(test);
DOCTEST_REGISTER_FUNCTION(test);

doctest{}
