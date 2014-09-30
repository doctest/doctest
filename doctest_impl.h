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
namespace doctestns
{

// matching of a string against a wildcard mask (case sensitivity configurable)
// taken from http://www.emoticode.net/c/simple-wildcard-string-compare-globbing-function.html
DOCTEST_INLINE int wildcmp(const char* str, const char* wild)
{
    const char* cp = 0, * mp = 0;

    while((*str) && (*wild != '*')) {
        if(
#ifdef DOCTEST_CASE_SENSITIVE
            (*wild != *str)
#else  // DOCTEST_CASE_SENSITIVE
            // rolled my own tolower() to not include more headers
            ((*wild >= 'A' && *wild <= 'Z') ? *wild + 32 : *wild) != ((*str >= 'A' && *str <= 'Z') ? *str + 32 : *str)
#endif // DOCTEST_CASE_SENSITIVE
            && (*wild != '?')) {
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
        } else if(
#ifdef DOCTEST_CASE_SENSITIVE
            (*wild == *str)
#else  // DOCTEST_CASE_SENSITIVE
            // rolled my own tolower() to not include more headers
            ((*wild >= 'A' && *wild <= 'Z') ? *wild + 32 : *wild) == ((*str >= 'A' && *str <= 'Z') ? *str + 32 : *str)
#endif // DOCTEST_CASE_SENSITIVE
            || (*wild == '?')) {
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
DOCTEST_INLINE int matchesAny(const char* name, char** filters, size_t filterCount, bool matchEmpty)
{
    if(filterCount == 0 && matchEmpty)
        return true;
    for(size_t i = 0; i < filterCount; ++i) {
        if(wildcmp(name, filters[i])) {
            return true;
        }
    }
    return false;
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
                filtersString = (char*)(malloc(len + 1));
                strcpy(filtersString, temp);
                break;
            }
        }
    }

    // if we have found the filter string
    if(filtersString) {
        const size_t maxFiltersInList = 1024; // ought to be enough
        filters = (char**)(malloc(sizeof(char*) * maxFiltersInList));
        // tokenize with "," as a separator for the first maxFiltersInList filters
        char* pch = strtok(filtersString, ",");
        while(pch != 0) {
            size_t len = strlen(pch);
            if(len && filterCount < maxFiltersInList) {
                filters[filterCount] = (char*)(malloc(len + 1));
                strcpy(filters[filterCount], pch);
                ++filterCount;
            }
            pch = strtok(0, ",");
        }
        free(filtersString);
    }
}

// the function type this library works with
typedef void (*funcType)(void);

// a struct defining a registered test callback
struct FunctionData {
    // fields by which difference of test functions shall be determined
    unsigned line;    // the line where the test was registered
    const char* file; // the file in which the test was registered

    // not used for comparing
    const char* suite;  // the test suite in which the test was added
    const char* name;   // name of the test function
    funcType f;         // a function pointer to the test function
    FunctionData* next; // a pointer to the next record in the current hash table bucket
};

// a comparison function for using qsort on arrays with pointers to FunctionData structures
DOCTEST_INLINE int functionDataComparator(const void* a, const void* b)
{
    FunctionData* lhs = *(FunctionData**)a;
    FunctionData* rhs = *(FunctionData**)b;

    int res = strcmp(lhs->file, rhs->file);
    if(res != 0)
        return res;
    return lhs->line - rhs->line;
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
DOCTEST_INLINE int registerFunction(funcType f, unsigned line, const char* file, const char* name)
{
    // register the hash table cleanup function only once
    static int cleanupFunctionRegistered = false;
    if(!cleanupFunctionRegistered) {
        atexit(cleanupHashTable);
        cleanupFunctionRegistered = true;
    }

    FunctionData** hashTable = getHashTable();
    // get the current test suite
    const char* suite = getTestSuiteName();

    // compute the hash using the file and the line at which the test was registered
    unsigned long hash = hashStr((unsigned const char*)file) ^ line;

    // try to find the function in the hash table
    bool found = false;
    FunctionData*& bucket = hashTable[hash % DOCTEST_HASH_TABLE_NUM_BUCKETS];
    FunctionData* curr = bucket;
    FunctionData* last = curr;
    while(curr != 0) {
        // compare by line, file and suite
        if(curr->line == line && strcmp(curr->file, file) == 0 && strcmp(curr->suite, suite) == 0) {
            found = true;
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
            bucket = (FunctionData*)malloc(sizeof(FunctionData));
            memcpy(bucket, &data, sizeof(FunctionData));
        } else {
            // append the record to the current bucket
            last->next = (FunctionData*)malloc(sizeof(FunctionData));
            memcpy(last->next, &data, sizeof(FunctionData));
        }
    }
    return 0;
}

DOCTEST_INLINE void invokeAllFunctions(int argc, char** argv)
{
    char** filters[6] = {0, 0, 0, 0, 0, 0};
    size_t counts[6] = {0, 0, 0, 0, 0, 0};
    parseArgs(argc, argv, "-doctest_file=", filters[0], counts[0]);
    parseArgs(argc, argv, "-doctest_file_exclude=", filters[1], counts[1]);
    parseArgs(argc, argv, "-doctest_suite=", filters[2], counts[2]);
    parseArgs(argc, argv, "-doctest_suite_exclude=", filters[3], counts[3]);
    parseArgs(argc, argv, "-doctest_name=", filters[4], counts[4]);
    parseArgs(argc, argv, "-doctest_name_exclude=", filters[5], counts[5]);

    // invoke the registered functions
    FunctionData** hashTable = getHashTable();
    size_t hashTableSize = getHashTableSize();
    FunctionData** hashEntryArray = (FunctionData**)malloc(hashTableSize * sizeof(FunctionData*));

    // fill the hashEntryArray with pointers to hash table records for sorting later
    size_t k = 0;
    for(size_t i = 0; i < DOCTEST_HASH_TABLE_NUM_BUCKETS; i++) {
        FunctionData* curr = hashTable[i];
        while(curr) {
            hashEntryArray[k++] = curr;
            curr = curr->next;
        }
    }

    // sort the collected records
    qsort(hashEntryArray, hashTableSize, sizeof(FunctionData*), functionDataComparator);

    // invoke the registered functions if they match the filter criteria
    for(size_t i = 0; i < hashTableSize; i++) {
        FunctionData& data = *hashEntryArray[i];
        if(!matchesAny(data.file, filters[0], counts[0], true))
            continue;
        if(matchesAny(data.file, filters[1], counts[1], false))
            continue;
        if(!matchesAny(data.suite, filters[2], counts[2], true))
            continue;
        if(matchesAny(data.suite, filters[3], counts[3], false))
            continue;
        if(!matchesAny(data.name, filters[4], counts[4], true))
            continue;
        if(matchesAny(data.name, filters[5], counts[5], false))
            continue;

        // call the function if it passes all the filtering
        try {
            data.f();
        } catch(std::exception& e) {
            // printf("%s\n", e.what());
            e.what();
        } catch(...) {
            // printf("Unknown exception caught!\n");
        }
    }

    // cleanup buffers
    for(size_t i = 0; i < 6; i++) {
        if(counts[i] > 0) {
            for(size_t k = 0; k < counts[i]; ++k)
                free(filters[i][k]);
            free(filters[i]);
        }
    }
}
} // namespace doctestns

// undef stuff defined at the top to keep things isolated
#ifdef UNDEFINE_CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#undef UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif // UNDEFINE_CRT_SECURE_NO_WARNINGS

#undef DOCTEST_INLINE

#endif // DOCTEST_GLOBAL_DISABLE
