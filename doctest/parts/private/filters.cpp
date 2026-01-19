#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/filters.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {
namespace detail {

    int wildcmp(const char* str, const char* wild, bool caseSensitive) {
        const char* cp = str;
        const char* mp = wild;

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
                wild = mp;   //!OCLINT parameter reassignment
                str  = cp++; //!OCLINT parameter reassignment
            }
        }

        while(*wild == '*') {
            wild++;
        }
        return !*wild;
    }

    bool matchesAny(const char* name, const std::vector<String>& filters, bool matchEmpty,
        bool caseSensitive) {
        if (filters.empty() && matchEmpty)
            return true;
        for (auto& curr : filters)
            if (wildcmp(name, curr.c_str(), caseSensitive))
                return true;
        return false;
    }

} // namespace detail
} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
