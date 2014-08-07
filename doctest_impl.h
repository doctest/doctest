#pragma once

#if !defined DOCTEST_GLOBAL_DISABLE

// if this header is included into the main doctest.h header the functions
// defined here should be marked as inline so compilation doesn't fail
#define DOCTEST_INLINE inline
#if defined DOCTEST_DONT_INCLUDE_IMPLEMENTATION
#undef DOCTEST_INLINE
#define DOCTEST_INLINE
#endif

// MSVC fix to not complain about using strcpy instead of strcpy_s
#if defined _MSC_VER
#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif
#endif

// required includes
#include <map>
#include <cstring>
#include <cstdlib> // for atexit

namespace doctestns {

    typedef void (*functionType)(void);

    struct functionSignature {
        unsigned line;
        const char* file;
        const char* method;

        const char* name; // not used for comparing

        bool operator<(const functionSignature& other) const {
            if(line != other.line)
                return line < other.line;
            int fileCmp = strcmp(file, other.file);
            if(fileCmp != 0)
                return fileCmp < 0;
            return strcmp(method, other.method) < 0;
        }
    };

    void cleanupRegisteredFunctions();

    // trick to register a global variable in a header - as a static var in an inline method
    DOCTEST_INLINE std::map<functionSignature, functionType>*& getRegisteredFunctions() {
        static std::map<functionSignature, functionType>* value = 0;
        // register the cleanup callback only for the first time this getter is called
        if(value == 0)
            atexit(cleanupRegisteredFunctions);
        return value;
    }

    // cleanup function for the container with doctest callbacks
    DOCTEST_INLINE void cleanupRegisteredFunctions() {
        std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
        delete registeredFunctions;
        registeredFunctions = 0;
    }

    // the function used by the macros for registering doctest callbacks (short
    // name for small codegen)
    DOCTEST_INLINE int r(functionType f, unsigned line, const char* file, const char* method,
                         const char* name) {
        std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
        // if being the first one to register a callback, initialize the storage
        if(!registeredFunctions)
            registeredFunctions = new std::map<functionSignature, functionType>;
        // initialize the record
        functionSignature signature;
        signature.line = line;
        signature.file = file;
        signature.method = method;
        signature.name = name;
        // insert the record
        registeredFunctions->insert(std::pair<functionSignature, functionType>(signature, f));
        return 0;
    }

    DOCTEST_INLINE void invokeAllFunctions(int argc, char** argv) {
        std::map<functionSignature, functionType>*& registeredFunctions = getRegisteredFunctions();
        // if atleast one test has been registered
        if(registeredFunctions) {
            char* filtersString = 0;
            char** filters = 0;
            unsigned filterCount = 0;
            // locate the filters string from the arguments (with comma
            // separated filters)
            for(int i = 1; i < argc; ++i) {
                const char* temp = strstr(argv[i], "-doctest=");
                if(temp) {
                    temp += strlen("-doctest=");
                    unsigned len = strlen(temp);
                    if(len) {
                        filtersString = new char[len + 1];
                        strcpy(filtersString, temp);
                        break;
                    }
                }
            }
            // if we have found the filter string
            if(filtersString) {
                const unsigned maxFiltersInCommaSeparatedList = 1024; // ought to be enough
                filters = new char* [maxFiltersInCommaSeparatedList];
                // tokenize with "," as a separator for the first
                // maxFiltersInCommaSeparatedList filters
                char* pch = strtok(filtersString, ",");
                while(pch != 0) {
                    unsigned len = strlen(pch);
                    if(len && filterCount < maxFiltersInCommaSeparatedList) {
                        filters[filterCount] = new char[len + 1];
                        strcpy(filters[filterCount], pch);
                        ++filterCount;
                    }
                    pch = strtok(0, ",");
                }
            }
            // invoke the registered functions
            std::map<functionSignature, functionType>::iterator it;
            for(it = registeredFunctions->begin(); it != registeredFunctions->end(); ++it) {
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
                    // just invoke each test if there are no filters
                    it->second();
                }
            }
            // cleanup buffers
            if(filters) {
                for(unsigned i = 0; i < filterCount; ++i)
                    delete[] filters[i];
                delete[] filters;
                delete[] filtersString;
            }
        }
    }
} // namespace doctestns

// undef stuff defined at the top to keep things isolated
#if defined UNDEFINE_CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#undef UNDEFINE_CRT_SECURE_NO_WARNINGS
#endif

#undef DOCTEST_INLINE

#endif // DOCTEST_GLOBAL_DISABLE
