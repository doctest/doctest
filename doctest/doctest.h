#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_STR_CONCAT_IMPL(s1, s2) s1##s2
#define DOCTEST_STR_CONCAT(s1, s2) DOCTEST_STR_CONCAT_IMPL(s1, s2)
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS(x) DOCTEST_STR_CONCAT(x, __COUNTER__)
#else // __COUNTER__
#define DOCTEST_ANONYMOUS(x) DOCTEST_STR_CONCAT(x, __LINE__)
#endif // __COUNTER__

// internal macro for making a string
#define DOCTEST_TOSTR_IMPL(x) #x
#define DOCTEST_TOSTR(x) DOCTEST_TOSTR_IMPL(x)

// internal macro for concatenating 2 literals and making the result a string
#define DOCTEST_STR_CONCAT_TOSTR(s1, s2) DOCTEST_TOSTR(DOCTEST_STR_CONCAT(s1, s2))

namespace doctest
{
// a dummy function that can be used for initializing globals (for silencing warnings)
inline int dummy()
{
    return 0;
}
} // namespace doctest_generated

// if registering is not disabled
#if !defined(DOCTEST_CONFIG_DISABLE)

#if !defined(DOCTEST_CONFIG_IMPLEMENT)
namespace doctest
{
namespace detail
{
    // forward declarations of the function used by the registering macros
    int regTest(void (*f)(void), unsigned line, const char* file, const char* name);
    int setTestSuiteName(const char* name);
} // namespace detail
// forward declarations of the functions intended for direct use
void* createParams(int argc, char** argv);
void addFilter(void* params_struct, const char* filter, const char* value);
void setOption(void* params_struct, const char* option, int value);
void freeParams(void* params_struct);
void setTestExecutionWrapper(void* params_struct, int (*f)(void (*)(void)));
int runTests(void* params_struct);
} // namespace doctest
#endif // DOCTEST_CONFIG_IMPLEMENT

// internal registering macros
#if !defined(__clang__)
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                           \
        doctest::detail::regTest(f, __LINE__, __FILE__, #name);
#else
// @TODO: try to do this for all global constructors so that we are warning-free
#define DOCTEST_REGISTER_FUNCTION(f, name)                                                         \
    _Pragma("clang diagnostic push")                                                               \
        _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"") static int                   \
            DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) =                                              \
                doctest::detail::regTest(f, __LINE__, __FILE__, #name);                            \
    _Pragma("clang diagnostic pop")
#endif

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace                                                                                      \
    {                                                                                              \
        struct der : base {                                                                        \
            void f();                                                                              \
        };                                                                                         \
        static void func()                                                                         \
        {                                                                                          \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
        DOCTEST_REGISTER_FUNCTION(func, name)                                                      \
    }                                                                                              \
    inline void der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    static void f();                                                                               \
    DOCTEST_REGISTER_FUNCTION(f, name)                                                             \
    inline void f()

// for registering doctests
#define DOCTEST_TEST(name)                                                                         \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)
#define DOCTEST_TEST_NONAME                                                                        \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), noname)

// for registering doctests with a fixture
#define DOCTEST_FIXTURE(c, name)                                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), c,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)
#define DOCTEST_FIXTURE_NONAME(c)                                                                  \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), c,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), noname)

// for starting a testsuite block
#define DOCTEST_TESTSUITE(name)                                                                    \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) = doctest::detail::setTestSuiteName(#name); \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_GIVE_ME_COMMA_) = doctest::dummy()

// for ending a testsuite block
#define DOCTEST_TESTSUITE_END                                                                      \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_VAR_) = doctest::detail::setTestSuiteName("");    \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_GIVE_ME_COMMA_) = doctest::dummy()

// =============================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!  ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_CONFIG_DISABLE GLOBALLY!        ==
// =============================================================================
#else // DOCTEST_CONFIG_DISABLE

// clang-format off
namespace doctest
{
inline void* createParams(int, char**) { return 0; }
inline void addFilter(void*, const char*, const char*) {}
inline void setOption(void*, const char*, int) {}
inline void freeParams(void*) {}
inline void setTestExecutionWrapper(void*, int (*)(void (*)(void))) {}
inline int runTests(void*) { return 0; }
} // namespace doctest
// clang-format on

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace                                                                                      \
    {                                                                                              \
        struct der : base {                                                                        \
            void f();                                                                              \
        };                                                                                         \
        static void func()                                                                         \
        {                                                                                          \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
    }                                                                                              \
    inline void der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name) static inline void f()

// for registering doctests
#define DOCTEST_TEST(name)                                                                         \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)
#define DOCTEST_TEST_NONAME                                                                        \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), noname)

// for registering doctests with a fixture
#define DOCTEST_FIXTURE(x, name)                                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), x,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), name)
#define DOCTEST_FIXTURE_NONAME(x)                                                                  \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_CLASS_), x,                        \
                              DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_FUNC_), noname)

// for starting a testsuite block
#define DOCTEST_TESTSUITE(name)                                                                    \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_GIVE_ME_COMMA_) = doctest::dummy()

// for ending a testsuite block
#define DOCTEST_TESTSUITE_END                                                                      \
    static int DOCTEST_ANONYMOUS(DOCTEST_AUTOGEN_GIVE_ME_COMMA_) = doctest::dummy()

#endif // DOCTEST_CONFIG_DISABLE

// == THIS SUPPLIES A MAIN FUNCTION AND SHOULD BE DONE ONLY IN ONE TRANSLATION UNIT
#ifdef DOCTEST_CONFIG_MAIN
// Standard C/C++ main entry point
int main(int argc, char** argv)
{
    void* params = doctest::createParams(argc, argv);
    int res = doctest::runTests(params);
    doctest::freeParams(params);
    return res;
}
#endif

#define doctest_test(name) DOCTEST_TEST(name)
#define doctest_test_noname DOCTEST_TEST_NONAME
#define doctest_fixture(c, name) DOCTEST_FIXTURE(c, name)
#define doctest_fixture_noname(c) DOCTEST_FIXTURE_NONAME(c)
#define doctest_testsuite(name) DOCTEST_TESTSUITE(name)
#define doctest_testsuite_end DOCTEST_TESTSUITE_END

// == SHORT VERSIONS OF THE TEST/FIXTURE/TESTSUITE MACROS
#ifdef DOCTEST_CONFIG_SHORT_MACRO_NAMES

#define TEST(name) DOCTEST_TEST(name)
#define TEST_NONAME DOCTEST_TEST_NONAME
#define FIXTURE(c, name) DOCTEST_FIXTURE(c, name)
#define FIXTURE_NONAME(c) DOCTEST_FIXTURE_NONAME(c)
#define TESTSUITE(name) DOCTEST_TESTSUITE(name)
#define TESTSUITE_END DOCTEST_TESTSUITE_END

#define test(name) doctest_test(name)
#define test_noname doctest_test_noname
#define fixture(c, name) doctest_fixture(c, name)
#define fixture_noname(c) doctest_fixture_noname(c)
#define testsuite(name) doctest_testsuite(name)
#define testsuite_end doctest_testsuite_end

#endif // DOCTEST_CONFIG_SHORT_MACRO_NAMES

#endif // DOCTEST_LIBRARY_INCLUDED

// =============================================================================
// == WHAT FOLLOWS IS THE IMPLEMENTATION OF THE TEST RUNNER                   ==
// =============================================================================
#if defined(DOCTEST_CONFIG_IMPLEMENT)
#ifndef DOCTEST_LIBRARY_IMPLEMENTATION
#define DOCTEST_LIBRARY_IMPLEMENTATION

// CLANG fix for defining non-inline functions in a header
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

// MSVC fix to not complain about using strcpy instead of strcpy_s and other such stuff
#ifdef _MSC_VER
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

// required includes
#include <cstdio>  // printf and friends
#include <cstdlib> // malloc, qsort
#include <cstring> // C string manipulation

// the number of buckets used for the hash set
#if !defined(DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS)
#define DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS 1024
#endif // DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS

// the number of buckets used for the hash set
#if !defined(DOCTEST_CONFIG_MAX_FILTERS_IN_LIST)
#define DOCTEST_CONFIG_MAX_FILTERS_IN_LIST 64
#endif // DOCTEST_CONFIG_MAX_FILTERS_IN_LIST

// main namespace of the library
namespace doctest
{

// the function type this library works with
typedef void (*funcType)(void);

// library internals namespace
namespace detail
{
    char tolower(const char c)
    {
        return ((c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c);
    }

    int strcicmp(char const* a, char const* b)
    {
        for(;; a++, b++) {
            int d = tolower(*a) - tolower(*b);
            if(d != 0 || !*a)
                return d;
        }
    }

    // matching of a string against a wildcard mask (case sensitivity configurable)
    // taken from http://www.emoticode.net/c/simple-wildcard-string-compare-globbing-function.html
    int wildcmp(const char* str, const char* wild, int caseSensitive)
    {
        const char* cp = 0, * mp = 0;

        // rolled my own tolower() to not include more headers
        while((*str) && (*wild != '*')) {
            if((caseSensitive ? (*wild != *str) : (tolower(*wild) != tolower(*str))) &&
               (*wild != '?')) {
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
            } else if((caseSensitive ? (*wild == *str) : (tolower(*wild) == tolower(*str))) ||
                      (*wild == '?')) {
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

    // C string hash function (djb2) - taken from http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hashStr(unsigned const char* str)
    {
        unsigned long hash = 5381;
        unsigned int c;

        while((c = *str++))
            hash = ((hash << 5) + hash) + c; // hash * 33 + c

        return hash;
    }

    // checks if the name matches any of the filters (and can be configured what to do when empty)
    int matchesAny(const char* name, char** filters, size_t filterCount, int matchEmpty,
                   int caseSensitive)
    {
        if(filterCount == 0 && matchEmpty)
            return 1;
        for(size_t i = 0; i < filterCount; ++i) {
            if(wildcmp(name, filters[i], caseSensitive)) {
                return 1;
            }
        }
        return 0;
    }

    // a struct defining a registered test callback
    struct FunctionData {
        // not used for comparing
        const char* suite;  // the test suite in which the test was added
        const char* name;   // name of the test function
        funcType f;         // a function pointer to the test function
        FunctionData* next; // a pointer to the next record in the current hash table bucket

        // fields by which difference of test functions shall be determined
        const char* file; // the file in which the test was registered
        unsigned line;    // the line where the test was registered

        int padding; // padding - not used for anything
    };

    // a comparison function for using qsort on arrays with pointers to FunctionData structures
    int functionDataComparator(const void* a, const void* b)
    {
        const FunctionData* lhs = *static_cast<FunctionData* const*>(a);
        const FunctionData* rhs = *static_cast<FunctionData* const*>(b);

        int res = strcmp(lhs->file, rhs->file);
        if(res != 0)
            return res;
        return static_cast<int>(lhs->line - rhs->line);
    }

    // the global hash table
    FunctionData** getHashTable()
    {
        static FunctionData* value[DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS];
        return value;
    }

    // the global hash table size
    size_t& getHashTableSize()
    {
        static size_t value;
        return value;
    }

    // trick to register a global variable in a header - as a static var in an inline method
    const char*& getTestSuiteName()
    {
        static const char* value = "";
        return value;
    }

    // sets the current test suite
    int setTestSuiteName(const char* name)
    {
        getTestSuiteName() = name;
        return 0;
    }

    // traverses the hash table bucket by bucket and frees everything
    void cleanupHashTable()
    {
        FunctionData** hashTable = getHashTable();
        for(size_t i = 0; i < DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS; i++) {
            FunctionData*& curr = hashTable[i];
            while(curr) {
                FunctionData* temp = curr->next;
                free(curr);
                curr = 0;
                curr = temp;
            }
        }
    }

    // used by the macros for registering doctest callbacks
    int regTest(funcType f, unsigned line, const char* file, const char* name)
    {
        // register the hash table cleanup function only once
        static int cleanupFunctionRegistered = 0;
        if(!cleanupFunctionRegistered) {
            atexit(cleanupHashTable);
            cleanupFunctionRegistered = 1;
        }

        FunctionData** hashTable = getHashTable();
        // get the current test suite
        const char* suite = getTestSuiteName();

        // compute the hash using the file and the line at which the test was registered
        unsigned long hash = hashStr(reinterpret_cast<unsigned const char*>(file)) ^ line;

        // try to find the function in the hash table
        int found = 0;
        FunctionData*& bucket = hashTable[hash % DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS];
        FunctionData* curr = bucket;
        FunctionData* last = curr;
        while(curr != 0) {
            // compare by line, file and suite
            if(curr->line == line && strcmp(curr->file, file) == 0) {
                found = 1;
                break;
            }
            last = curr;
            curr = curr->next;
        }

        // if not found in the bucket
        if(!found) {
            getHashTableSize()++;

            // initialize the record
            FunctionData data;
            data.line = line;
            data.file = file;
            data.suite = suite;
            data.name = name;
            data.f = f;
            data.next = 0;

            if(last == 0) {
                // insert the record into this bucket as a first item
                bucket = static_cast<FunctionData*>(malloc(sizeof(FunctionData)));
                memcpy(bucket, &data, sizeof(FunctionData));
            } else {
                // append the record to the current bucket
                last->next = static_cast<FunctionData*>(malloc(sizeof(FunctionData)));
                memcpy(last->next, &data, sizeof(FunctionData));
            }
        }
        return 0;
    }

    // this is needed because MSVC does not permit mixing 2 exception handling schemes in a function
    int callTestFunc(int (*testExecutionWrapper)(funcType), funcType f)
    {
        try {
            if(testExecutionWrapper) {
                return testExecutionWrapper(f);
            } else {
                f();
            }
        } catch(...) {
            printf("Unknown exception caught!\n");
            return 1;
        }
        return 0;
    }

    struct Parameters {
        char** filters[6];
        size_t filterCounts[6];

        int count;                // if only the count of matching tests is to be retreived
        int case_sensitive;       // if filtering should be case sensitive
        int separate_process;     // if each test should be executed in a separate process
        int allow_overrides;      // all but this can be overriden
        int exit_after_tests;     // calls exit() after the tests are ran/counted
        int first;                // the first (matching) test to be executed
        int last;                 // the last (matching) test to be executed
        int hash_table_histogram; // if the hash table should be printed as a histogram
        int no_run;               // to not run the tests at all (can be done with an "*" exclude)

        int padding; // padding - not used for anything

        int (*testExecutionWrapper)(funcType); // wrapper for test execution
    };

    // parses a comma separated list of words after a pattern in one of the arguments in argv
    void parseFilter(int argc, char** argv, const char* pattern, char**& filters,
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
            // tokenize with "," as a separator for the first DOCTEST_CONFIG_MAX_FILTERS_IN_LIST
            // filters
            char* pch = strtok(filtersString, ",");
            while(pch != 0) {
                size_t len = strlen(pch);
                if(len && filterCount < DOCTEST_CONFIG_MAX_FILTERS_IN_LIST) {
                    filters[filterCount] = static_cast<char*>(malloc(len + 1));
                    strcpy(filters[filterCount], pch);
                    ++filterCount;
                }
                pch = strtok(0, ",");
            }
            free(filtersString);
        }
    }

    // parses an option from the command line (bool: type == 0, int: type == 1)
    int parseOption(int argc, char** argv, const char* option, int type, int defaultVal)
    {
        int outVal = defaultVal;

        char** theStr =
            static_cast<char**>(malloc(sizeof(char*) * DOCTEST_CONFIG_MAX_FILTERS_IN_LIST));
        size_t theCount = 0;
        parseFilter(argc, argv, option, theStr, theCount);
        // if the option has been found
        if(theCount > 0) {
            // if there is only 1 value in the "comma separated list"
            if(theCount == 1) {
                if(type == 0) {
                    // boolean
                    const char positive[][5] = {"1", "true", "on", "yes"}; // 5 - strlen("true") + 1
                    const char negative[][6] = {"0", "false", "off", "no"};

                    // if the value matches any of the positive/negative possibilities
                    for(size_t i = 0; i < 4; i++) {
                        if(strcicmp(theStr[0], positive[i]) == 0) {
                            outVal = 1;
                            break;
                        }
                        if(strcicmp(theStr[0], negative[i]) == 0) {
                            outVal = 0;
                            break;
                        }
                    }
                } else {
                    // integer
                    int res = atoi(theStr[0]);
                    if(res != 0)
                        outVal = res;
                }
            }
        }

        // free the buffers
        for(size_t i = 0; i < theCount; i++)
            free(theStr[i]);
        free(theStr);

        return outVal;
    }

} // namespace detail

// constructs a type-erased params structure from the command line
void* createParams(int argc, char** argv)
{
    using namespace detail;

    detail::Parameters* params =
        static_cast<detail::Parameters*>(malloc(sizeof(detail::Parameters)));
    for(int i = 0; i < 6; i++) {
        params->filters[i] =
            static_cast<char**>(malloc(sizeof(char*) * DOCTEST_CONFIG_MAX_FILTERS_IN_LIST));
        params->filterCounts[i] = 0;
    }

    params->testExecutionWrapper = 0;

    parseFilter(argc, argv, "-dt-file=", params->filters[0], params->filterCounts[0]);
    parseFilter(argc, argv, "-dt-file-exclude=", params->filters[1], params->filterCounts[1]);
    parseFilter(argc, argv, "-dt-suite=", params->filters[2], params->filterCounts[2]);
    parseFilter(argc, argv, "-dt-suite-exclude=", params->filters[3], params->filterCounts[3]);
    parseFilter(argc, argv, "-dt-name=", params->filters[4], params->filterCounts[4]);
    parseFilter(argc, argv, "-dt-name-exclude=", params->filters[5], params->filterCounts[5]);

    params->count = parseOption(argc, argv, "-dt-count=", 0, 0);
    params->case_sensitive = parseOption(argc, argv, "-dt-case-sensitive=", 0, 0);
    params->allow_overrides = parseOption(argc, argv, "-dt-override=", 0, 1);
    params->separate_process = parseOption(argc, argv, "-dt-separate-process=", 0, 0);
    params->exit_after_tests = parseOption(argc, argv, "-dt-exit=", 0, 0);
    params->first = parseOption(argc, argv, "-dt-first=", 1, 1);
    params->last = parseOption(argc, argv, "-dt-last=", 1, 0);
    params->hash_table_histogram = parseOption(argc, argv, "-dt-hash-table-histogram=", 0, 0);
    params->no_run = parseOption(argc, argv, "-dt-no-run=", 0, 0);

    return static_cast<void*>(params);
}

// allows the user to add procedurally to the filters from the command line
void addFilter(void* params_struct, const char* filter, const char* value)
{
    using namespace detail;

    Parameters* params = static_cast<Parameters*>(params_struct);

    if(params->allow_overrides) {
        size_t idx = 666;
        if(strcmp(filter, "dt-file") == 0)
            idx = 0;
        if(strcmp(filter, "dt-file-exclude") == 0)
            idx = 1;
        if(strcmp(filter, "dt-suite") == 0)
            idx = 2;
        if(strcmp(filter, "dt-suite-exclude") == 0)
            idx = 3;
        if(strcmp(filter, "dt-name") == 0)
            idx = 4;
        if(strcmp(filter, "dt-name-exclude") == 0)
            idx = 5;
        // if the filter name is valid
        if(idx != 666) {
            size_t len = strlen(value);
            size_t count = params->filterCounts[idx];
            // if there is place for more filters
            if(len > 0 && count < DOCTEST_CONFIG_MAX_FILTERS_IN_LIST) {
                // insert the filter in the array
                params->filters[idx][count] = static_cast<char*>(malloc(len + 1));
                strcpy(params->filters[idx][count], value);
                ++params->filterCounts[idx];
            }
        }
    }
}

// allows the user to override procedurally the options from the command line
void setOption(void* params_struct, const char* option, int value)
{
    using namespace detail;

    Parameters* params = static_cast<Parameters*>(params_struct);

    if(params->allow_overrides) {
        if(strcmp(option, "dt-count") == 0)
            params->count = value;
        if(strcmp(option, "dt-case-sensitive") == 0)
            params->case_sensitive = value;
        if(strcmp(option, "dt-separate-process") == 0)
            params->separate_process = value;
        if(strcmp(option, "dt-exit") == 0)
            params->exit_after_tests = value;
        if(strcmp(option, "dt-first") == 0)
            params->first = value;
        if(strcmp(option, "dt-last") == 0)
            params->last = value;
        if(strcmp(option, "dt-hash-table-histogram") == 0)
            params->hash_table_histogram = value;
        if(strcmp(option, "dt-no-run") == 0)
            params->no_run = value;
    }
}

// allows the user to provide a test execution wrapper for custom exception handling
void setTestExecutionWrapper(void* params_struct, int (*f)(funcType))
{
    using namespace detail;

    Parameters* params = static_cast<Parameters*>(params_struct);

    params->testExecutionWrapper = f;
}

// frees the type-erased params structure
void freeParams(void* params_struct)
{
    using namespace detail;

    Parameters* params = static_cast<Parameters*>(params_struct);

    // cleanup buffers
    for(size_t i = 0; i < 6; i++) {
        for(size_t k = 0; k < params->filterCounts[i]; ++k)
            free(params->filters[i][k]);
        free(params->filters[i]);
    }

    free(params);
}

// the main function that does all the filtering and test running
int runTests(void* params_struct)
{
    using namespace detail;

    Parameters* params = static_cast<Parameters*>(params_struct);

    // exit right now
    if(params->no_run)
        return 0;

    // invoke the registered functions
    FunctionData** hashTable = getHashTable();
    size_t hashTableSize = getHashTableSize();
    FunctionData** hashEntryArray =
        static_cast<FunctionData**>(malloc(hashTableSize * sizeof(FunctionData*)));

    // fill the hashEntryArray with pointers to hash table records for sorting later
    size_t numTestsSoFar = 0;
    for(size_t i = 0; i < DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS; i++) {
        FunctionData* curr = hashTable[i];
        while(curr) {
            hashEntryArray[numTestsSoFar++] = curr;
            curr = curr->next;
        }
    }

    // sort the collected records
    qsort(hashEntryArray, hashTableSize, sizeof(FunctionData*), functionDataComparator);

    // if(params->printHashTableHistogram) {
    if(params->hash_table_histogram) {
        // find the most full bucket
        int maxElementsInBucket = 0;
        for(size_t i = 0; i < DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS; i++) {
            FunctionData* curr = hashTable[i];
            int count = 0;
            while(curr) {
                count++;
                curr = curr->next;
            }
            if(count > maxElementsInBucket)
                maxElementsInBucket = count;
        }

        // print a prettified histogram
        printf("[doctest] hash table bucket histogram\n");
        printf("============================================================\n");
        printf("#bucket     |count| relative count\n");
        printf("============================================================\n");
        for(size_t i = 0; i < DOCTEST_CONFIG_HASH_TABLE_NUM_BUCKETS; i++) {
            FunctionData* curr = hashTable[i];
            int count = 0;
            while(curr) {
                count++;
                curr = curr->next;
            }
            printf("bucket %4d |%4d |", static_cast<int>(i), count);

            float ratio = static_cast<float>(count) / static_cast<float>(maxElementsInBucket);
            int numStars = static_cast<int>(ratio * 41);
            for(int k = 0; k < numStars; ++k)
                printf("*");
            printf("\n");
        }
        printf("\n");
    }

    int caseSensitive = params->case_sensitive;
    int numFilterPassedTests = 0;
    int numFailed = 0;
    // invoke the registered functions if they match the filter criteria (or just count them)
    for(size_t i = 0; i < hashTableSize; i++) {
        FunctionData& data = *hashEntryArray[i];
        if(!matchesAny(data.file, params->filters[0], params->filterCounts[0], 1, caseSensitive))
            continue;
        if(matchesAny(data.file, params->filters[1], params->filterCounts[1], 0, caseSensitive))
            continue;
        if(!matchesAny(data.suite, params->filters[2], params->filterCounts[2], 1, caseSensitive))
            continue;
        if(matchesAny(data.suite, params->filters[3], params->filterCounts[3], 0, caseSensitive))
            continue;
        if(!matchesAny(data.name, params->filters[4], params->filterCounts[4], 1, caseSensitive))
            continue;
        if(matchesAny(data.name, params->filters[5], params->filterCounts[5], 0, caseSensitive))
            continue;

        numFilterPassedTests++;
        // do not execute the test if we are to only count the number of filter passing tests
        if(params->count)
            continue;

        // skip the test if it is not in the execution range
        if((params->last < numFilterPassedTests && params->first <= params->last) ||
           (params->first > numFilterPassedTests))
            continue;

        // execute the test if it passes all the filtering
        {
            int res = 0;

#ifdef _MSC_VER
            __try
            {
#endif // _MSC_VER
                res = callTestFunc(params->testExecutionWrapper, data.f);
#ifdef _MSC_VER
            }
            __except(1)
            {
                printf("Unknown SEH exception caught!\n");
                res = 1;
            }
#endif // _MSC_VER

            if(res) {
                printf("Test failed!\n");
                numFailed++;
            }
        }
    }

    if(params->count)
        printf("[doctest] number of registered tests passing the current filters: %d\n",
               numFilterPassedTests);

    // cleanup buffers
    free(hashEntryArray);

    if(params->exit_after_tests) {
        // @NOTE: this will fail if the user has wrapped this in a RAII thing or in atexit()
        freeParams(params_struct);
        exit(numFailed); // @TODO: is this legal? Or should I only pass EXIT_SUCCESS/EXIT_FAILURE?
    }

    return numFailed;
}
} // namespace doctest

// undef stuff defined at the top to keep things isolated
#ifdef UNDEFINE_CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#undef UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // UNDEFINE_CRT_SECURE_NO_WARNINGS

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // DOCTEST_LIBRARY_IMPLEMENTATION
#endif // DOCTEST_CONFIG_IMPLEMENT
