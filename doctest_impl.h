#pragma once

// if registering is not disabled
#if !defined(DOCTEST_GLOBAL_DISABLE)

// MSVC fix to not complain about using strcpy instead of strcpy_s and other such stuff
#ifdef _MSC_VER
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER



#include <cstdio>


// required includes
#include <cstdlib> // malloc, qsort
#include <cstring> // C string manipulation
#include <exception>

// if this header is included into the main doctest.h header the functions
// defined here should be marked as inline so compilation doesn't fail
#define DOCTEST_INLINE inline
#ifdef DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#undef DOCTEST_INLINE
#define DOCTEST_INLINE
#endif // DOCTEST_DONT_INCLUDE_IMPLEMENTATION

// the number of buckets used for the hash set
#if !defined(DOCTEST_HASH_TABLE_NUM_BUCKETS)
#define DOCTEST_HASH_TABLE_NUM_BUCKETS 8192
#endif // DOCTEST_HASH_TABLE_NUM_BUCKETS

// the number of buckets used for the hash set
#if !defined(DOCTEST_MAX_FILTERS_IN_LIST)
#define DOCTEST_MAX_FILTERS_IN_LIST 1024
#endif // DOCTEST_MAX_FILTERS_IN_LIST

// main namespace of the library
namespace doctest
{

// the function type this library works with
typedef void (*funcType)(void);

// library internals namespace
namespace detail
{
    // matching of a string against a wildcard mask (case sensitivity configurable)
    // taken from http://www.emoticode.net/c/simple-wildcard-string-compare-globbing-function.html
    DOCTEST_INLINE int wildcmp(const char* str, const char* wild, int caseSensitive)
    {
        const char* cp = 0, * mp = 0;

        // rolled my own tolower() to not include more headers
        while((*str) && (*wild != '*')) {
            if(caseSensitive ? (*wild != *str)
                             : (((*wild >= 'A' && *wild <= 'Z') ? *wild + 32 : *wild) !=
                                ((*str >= 'A' && *str <= 'Z') ? *str + 32 : *str)) &&
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
            } else if(caseSensitive ? (*wild == *str)
                                    : (((*wild >= 'A' && *wild <= 'Z') ? *wild + 32 : *wild) ==
                                       ((*str >= 'A' && *str <= 'Z') ? *str + 32 : *str)) ||
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
    DOCTEST_INLINE unsigned long hashStr(unsigned const char* str)
    {
        unsigned long hash = 5381;
        unsigned int c;

        while((c = *str++))
            hash = ((hash << 5) + hash) + c; // hash * 33 + c

        return hash;
    }

    // checks if the name matches any of the filters (and can be configured what to do when empty)
    DOCTEST_INLINE int matchesAny(const char* name, char** filters, size_t filterCount,
                                  int matchEmpty, int caseSensitive)
    {
        if(filterCount == 0 && matchEmpty)
            return 1;
        for(size_t i = 0; i < filterCount; ++i) {
            if(wildcmp(name, filters[i], caseSensitive)) {
                return 1;
            }
        }
        return 1;
    }

    // a struct defining a registered test callback
    struct FunctionData {
        // fields by which difference of test functions shall be determined
        const char* file; // the file in which the test was registered
        unsigned line;    // the line where the test was registered

        // not used for comparing
        const char* suite;  // the test suite in which the test was added
        const char* name;   // name of the test function
        funcType f;         // a function pointer to the test function
        FunctionData* next; // a pointer to the next record in the current hash table bucket
    };

    // a comparison function for using qsort on arrays with pointers to FunctionData structures
    DOCTEST_INLINE int functionDataComparator(const void* a, const void* b)
    {
        const FunctionData* lhs = *DOCTEST_STATIC_CAST(FunctionData * const*)(a);
        const FunctionData* rhs = *DOCTEST_STATIC_CAST(FunctionData * const*)(b);

        int res = strcmp(lhs->file, rhs->file);
        if(res != 0)
            return res;
        return DOCTEST_STATIC_CAST(int)(lhs->line - rhs->line);
    }

    // the global hash table
    DOCTEST_INLINE FunctionData** getHashTable()
    {
        static FunctionData* value[DOCTEST_HASH_TABLE_NUM_BUCKETS];
        return value;
    }

    // the global hash table size
    DOCTEST_INLINE size_t& getHashTableSize()
    {
        static size_t value;
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

    // traverses the hash table bucket by bucket and frees everything
    DOCTEST_INLINE void cleanupHashTable()
    {
        FunctionData** hashTable = getHashTable();
        for(size_t i = 0; i < DOCTEST_HASH_TABLE_NUM_BUCKETS; i++) {
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
    DOCTEST_INLINE int regTest(funcType f, unsigned line, const char* file, const char* name)
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
        unsigned long hash = hashStr(DOCTEST_REINTERPRET_CAST(unsigned const char*)(file)) ^ line;

        // try to find the function in the hash table
        int found = 0;
        FunctionData*& bucket = hashTable[hash % DOCTEST_HASH_TABLE_NUM_BUCKETS];
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
                bucket = DOCTEST_STATIC_CAST(FunctionData*)(malloc(sizeof(FunctionData)));
                memcpy(bucket, &data, sizeof(FunctionData));
            } else {
                // append the record to the current bucket
                last->next = DOCTEST_STATIC_CAST(FunctionData*)(malloc(sizeof(FunctionData)));
                memcpy(last->next, &data, sizeof(FunctionData));
            }
        }
        return 0;
    }

    // this is needed because MSVC does not permit mixing 2 exception handling schemes in a function
    DOCTEST_INLINE void callTestFunc(funcType f)
    {
        try {
            f();
        } catch(std::exception& e) {
            // printf("%s\n", e.what());
            e.what();
        } catch(...) {
            // printf("Unknown exception caught!\n");
        }
    }

    struct Parameters {
        char** filters[6];
        size_t filterCounts[6];

        // options
        int getCount;
        int caseSensitive;
        int allowOverrides;
    };

    // parses a comma separated list of words after a pattern in one of the arguments in argv
    DOCTEST_INLINE void parseFilter(int argc, char** argv, const char* pattern, char**& filters,
                                    size_t& filterCount)
    {
        char* filtersString = 0;
        for(int i = 0; i < argc; ++i) {
            const char* temp = strstr(argv[i], pattern);
            if(temp) {
                temp += strlen(pattern);
                size_t len = strlen(temp);
                if(len) {
                    filtersString = DOCTEST_STATIC_CAST(char*)(malloc(len + 1));
                    strcpy(filtersString, temp);
                    break;
                }
            }
        }

        // if we have found the filter string
        if(filtersString) {
            // tokenize with "," as a separator for the first DOCTEST_MAX_FILTERS_IN_LIST filters
            char* pch = strtok(filtersString, ",");
            while(pch != 0) {
                size_t len = strlen(pch);
                if(len && filterCount < DOCTEST_MAX_FILTERS_IN_LIST) {
                    filters[filterCount] = DOCTEST_STATIC_CAST(char*)(malloc(len + 1));
                    strcpy(filters[filterCount], pch);
                    ++filterCount;
                }
                pch = strtok(0, ",");
            }
            free(filtersString);
        }
    }

    // parses a bool option from the command line
    DOCTEST_INLINE int parseOption(int argc, char** argv, const char* option, int defaultVal)
    {
        int outVal = defaultVal;

        char** theStr = 0;
        size_t theCount = 0;
        parseFilter(argc, argv, option, theStr, theCount);
        // if the option has been found
        if(theCount > 0) {
            // if there is only 1 value in the "comma separated list"
            if(theCount == 1) {
                const char positive[][7] = {"1", "true", "TRUE", "on", "ON", "yes", "YES"};
                const char negative[][7] = {"0", "false", "FALSE", "off", "OFF", "no", "NO"};

                // if the value matches any of the positive/negative possibilities
                for(size_t i = 0; i < 7; i++) {
                    if(strcmp(theStr[0], positive[i]) == 0) {
                        outVal = 1;
                        break;
                    }
                    if(strcmp(theStr[0], negative[i]) == 0) {
                        outVal = 0;
                        break;
                    }
                }
            }

            // free the buffers
            for(size_t i = 0; i < theCount; i++)
                free(theStr[i]);
            free(theStr);
        }

        return outVal;
    }

} // namespace detail

// constructs a type-erased params structure from the command line
DOCTEST_INLINE void* createParams(int argc, char** argv)
{
    using namespace detail;

    detail::Parameters* params =
        DOCTEST_STATIC_CAST(detail::Parameters*)(malloc(sizeof(detail::Parameters)));
    for(int i = 0; i < 6; i++) {
        params->filters[i] =
            DOCTEST_STATIC_CAST(char**)(malloc(sizeof(char*) * DOCTEST_MAX_FILTERS_IN_LIST));
        params->filterCounts[i] = 0;
    }

    parseFilter(argc, argv, "-doctest_file=", params->filters[0], params->filterCounts[0]);
    parseFilter(argc, argv, "-doctest_file_exclude=", params->filters[1], params->filterCounts[1]);
    parseFilter(argc, argv, "-doctest_suite=", params->filters[2], params->filterCounts[2]);
    parseFilter(argc, argv, "-doctest_suite_exclude=", params->filters[3], params->filterCounts[3]);
    parseFilter(argc, argv, "-doctest_name=", params->filters[4], params->filterCounts[4]);
    parseFilter(argc, argv, "-doctest_name_exclude=", params->filters[5], params->filterCounts[5]);

    params->getCount = parseOption(argc, argv, "-doctest_count=", 0);
    params->caseSensitive = parseOption(argc, argv, "-doctest_case_sensitive=", 0);
    params->allowOverrides = parseOption(argc, argv, "-doctest_override=", 1);

    return DOCTEST_STATIC_CAST(void*)(params);
}

// allows the user to add procedurally to the filters from the command line
DOCTEST_INLINE void addFilter(void* params_struct, const char* filter, const char* value)
{
    using namespace detail;

    Parameters* params = DOCTEST_STATIC_CAST(Parameters*)(params_struct);

    if(params->allowOverrides) {
        size_t idx = 666;
        if(strcmp(filter, "doctest_file") == 0)
            idx = 0;
        if(strcmp(filter, "doctest_file_exclude") == 0)
            idx = 1;
        if(strcmp(filter, "doctest_suite") == 0)
            idx = 2;
        if(strcmp(filter, "doctest_suite_exclude") == 0)
            idx = 3;
        if(strcmp(filter, "doctest_name") == 0)
            idx = 4;
        if(strcmp(filter, "doctest_name_exclude") == 0)
            idx = 5;
        // if the filter name is valid
        if(idx != 666) {
            size_t len = strlen(value);
            size_t count = params->filterCounts[idx];
            // if there is place for more filters
            if(len > 0 && count < DOCTEST_MAX_FILTERS_IN_LIST) {
                // insert the filter in the array
                params->filters[idx][count] = DOCTEST_STATIC_CAST(char*)(malloc(len + 1));
                strcpy(params->filters[idx][count], value);
                ++params->filterCounts[idx];
            }
        }
    }
}

// allows the user to override procedurally the options from the command line
DOCTEST_INLINE void setOption(void* params_struct, const char* option, int value)
{
    using namespace detail;

    Parameters* params = DOCTEST_STATIC_CAST(Parameters*)(params_struct);

    if(params->allowOverrides) {
        if(strcmp(option, "doctest_count") == 0)
            params->getCount = value;
        if(strcmp(option, "doctest_case_sensitive") == 0)
            params->caseSensitive = value;
    }
}

// frees the type-erased params structure
DOCTEST_INLINE void freeParams(void* params_struct)
{
    using namespace detail;

    Parameters* params = DOCTEST_STATIC_CAST(Parameters*)(params_struct);

    // cleanup buffers
    for(size_t i = 0; i < 6; i++) {
        for(size_t k = 0; k < params->filterCounts[i]; ++k)
            free(params->filters[i][k]);
        free(params->filters[i]);
    }

    free(params);
}

DOCTEST_INLINE void runTests(void* params_struct)
{
    using namespace detail;

    Parameters* params = DOCTEST_STATIC_CAST(Parameters*)(params_struct);

    // invoke the registered functions
    FunctionData** hashTable = getHashTable();
    size_t hashTableSize = getHashTableSize();
    FunctionData** hashEntryArray =
        DOCTEST_STATIC_CAST(FunctionData**)(malloc(hashTableSize * sizeof(FunctionData*)));

    // fill the hashEntryArray with pointers to hash table records for sorting later
    size_t numTestsSoFar = 0;
    for(size_t i = 0; i < DOCTEST_HASH_TABLE_NUM_BUCKETS; i++) {
        FunctionData* curr = hashTable[i];
        while(curr) {
            hashEntryArray[numTestsSoFar++] = curr;
            curr = curr->next;
        }
    }

    // sort the collected records
    qsort(hashEntryArray, hashTableSize, sizeof(FunctionData*), functionDataComparator);

    int caseSensitive = params->caseSensitive;
    int numUnfilteredTests = 0;
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

        numUnfilteredTests++;
        if(params->getCount)
            continue;

#ifdef _MSC_VER
        __try
        {
#endif // _MSC_VER
            // call the function if it passes all the filtering
            callTestFunc(data.f);
#ifdef _MSC_VER
        }
        __except(1)
        {
            // printf("Unknown SEH exception caught!\n");
        }
#endif // _MSC_VER
    }

    // cleanup buffers
    free(hashEntryArray);
}
} // namespace doctest

// undef stuff defined at the top to keep things isolated
#ifdef UNDEFINE_CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#undef UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // UNDEFINE_CRT_SECURE_NO_WARNINGS

#undef DOCTEST_INLINE

#endif // DOCTEST_GLOBAL_DISABLE
