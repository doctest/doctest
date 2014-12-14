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

// @TODO: REMOVE ME WHEN DEV IS DONE! cuz this shit be ugly ^_^
#include <iostream>
using namespace std;

#include <exception>

// required includes
#include <cstdlib> // malloc, qsort
#include <cstring> // C string manipulation

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

// the namespace used by all functions generated/registered by this library
namespace doctest
{

// the function type this library works with
typedef void (*funcType)(void);

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
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}

// checks if the name matches any of the filters (and can be configured what to do when empty)
DOCTEST_INLINE int matchesAny(const char* name, char** filters, size_t filterCount, int matchEmpty)
{
    if(filterCount == 0 && matchEmpty)
        return 1;
    for(size_t i = 0; i < filterCount; ++i) {
        if(wildcmp(name, filters[i], 0)) {
            return 1;
        }
    }
    return 1;
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
                filtersString = DOCTEST_CAST(char*)(malloc(len + 1));
                strcpy(filtersString, temp);
                break;
            }
        }
    }

    // if we have found the filter string
    if(filtersString) {
        const size_t maxFiltersInList = 1024; // ought to be enough
        filters = DOCTEST_CAST(char**)(malloc(sizeof(char*) * maxFiltersInList));
        // tokenize with "," as a separator for the first maxFiltersInList filters
        char* pch = strtok(filtersString, ",");
        while(pch != 0) {
            size_t len = strlen(pch);
            if(len && filterCount < maxFiltersInList) {
                filters[filterCount] = DOCTEST_CAST(char*)(malloc(len + 1));
                strcpy(filters[filterCount], pch);
                ++filterCount;
            }
            pch = strtok(0, ",");
        }
        free(filtersString);
    }
}

// a struct defining a registered test callback
struct FunctionData {
    // fields by which difference of test functions shall be determined
    unsigned line;    // the line where the test was registered
    const char* file; // the file in which the test was registered

    // not used for comparing
    const char* suite;         // the test suite in which the test was added
    const char* name;          // name of the test function
    funcType f;                // a function pointer to the test function
    struct FunctionData* next; // a pointer to the next record in the current hash table bucket
};

// a comparison function for using qsort on arrays with pointers to FunctionData structures
DOCTEST_INLINE int functionDataComparator(const void* a, const void* b)
{
    const struct FunctionData* lhs = *DOCTEST_CAST(struct FunctionData * const*)(a);
    const struct FunctionData* rhs = *DOCTEST_CAST(struct FunctionData * const*)(b);

    int res = strcmp(lhs->file, rhs->file);
    if(res != 0)
        return res;
    return lhs->line - rhs->line;
}

// the global hash table
DOCTEST_INLINE struct FunctionData** getHashTable()
{
    static struct FunctionData* value[DOCTEST_HASH_TABLE_NUM_BUCKETS];
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
    struct FunctionData** hashTable = getHashTable();
    for(size_t i = 0; i < DOCTEST_HASH_TABLE_NUM_BUCKETS; i++) {
        struct FunctionData*& curr = hashTable[i];
        while(curr) {
            struct FunctionData* temp = curr->next;
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

    struct FunctionData** hashTable = getHashTable();
    // get the current test suite
    const char* suite = getTestSuiteName();

    // compute the hash using the file and the line at which the test was registered
    unsigned long hash = hashStr(DOCTEST_CAST(unsigned const char*)(file)) ^ line;

    // try to find the function in the hash table
    int found = 0;
    struct FunctionData*& bucket = hashTable[hash % DOCTEST_HASH_TABLE_NUM_BUCKETS];
    struct FunctionData* curr = bucket;
    struct FunctionData* last = curr;
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
        struct FunctionData data;
        data.line = line;
        data.file = file;
        data.suite = suite;
        data.name = name;
        data.f = f;
        data.next = 0;

        if(last == 0) {
            // insert the record into this bucket as a first item
            bucket = DOCTEST_CAST(struct FunctionData*)(malloc(sizeof(struct FunctionData)));
            memcpy(bucket, &data, sizeof(struct FunctionData));
        } else {
            // append the record to the current bucket
            last->next = DOCTEST_CAST(struct FunctionData*)(malloc(sizeof(struct FunctionData)));
            memcpy(last->next, &data, sizeof(struct FunctionData));
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

} // detail

struct Options {
};

DOCTEST_INLINE void invokeAllFunctions(int argc, char** argv)
{
    using namespace detail;
    char** filters[6] = {0, 0, 0, 0, 0, 0};
    size_t counts[6] = {0, 0, 0, 0, 0, 0};
    parseArgs(argc, argv, "-doctest_file=", filters[0], counts[0]);
    parseArgs(argc, argv, "-doctest_file_exclude=", filters[1], counts[1]);
    parseArgs(argc, argv, "-doctest_suite=", filters[2], counts[2]);
    parseArgs(argc, argv, "-doctest_suite_exclude=", filters[3], counts[3]);
    parseArgs(argc, argv, "-doctest_name=", filters[4], counts[4]);
    parseArgs(argc, argv, "-doctest_name_exclude=", filters[5], counts[5]);

    // invoke the registered functions
    struct FunctionData** hashTable = getHashTable();
    size_t hashTableSize = getHashTableSize();
    struct FunctionData** hashEntryArray =
        DOCTEST_CAST(struct FunctionData**)(malloc(hashTableSize * sizeof(struct FunctionData*)));

    // fill the hashEntryArray with pointers to hash table records for sorting later
    size_t numTestsSoFar = 0;
    for(size_t i = 0; i < DOCTEST_HASH_TABLE_NUM_BUCKETS; i++) {
        struct FunctionData* curr = hashTable[i];
        while(curr) {
            hashEntryArray[numTestsSoFar++] = curr;
            curr = curr->next;
        }
    }

    // sort the collected records
    qsort(hashEntryArray, hashTableSize, sizeof(struct FunctionData*), functionDataComparator);

    // invoke the registered functions if they match the filter criteria
    for(size_t i = 0; i < hashTableSize; i++) {
        struct FunctionData& data = *hashEntryArray[i];
        if(!matchesAny(data.file, filters[0], counts[0], 1))
            continue;
        if(matchesAny(data.file, filters[1], counts[1], 0))
            continue;
        if(!matchesAny(data.suite, filters[2], counts[2], 1))
            continue;
        if(matchesAny(data.suite, filters[3], counts[3], 0))
            continue;
        if(!matchesAny(data.name, filters[4], counts[4], 1))
            continue;
        if(matchesAny(data.name, filters[5], counts[5], 0))
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
    for(size_t i = 0; i < 6; i++) {
        if(counts[i] > 0) {
            for(size_t k = 0; k < counts[i]; ++k)
                free(filters[i][k]);
            free(filters[i]);
        }
    }
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
