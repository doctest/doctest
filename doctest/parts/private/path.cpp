#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR
#define DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR ':'
#endif

namespace doctest {

DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wnull-dereference")
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wnull-dereference")
// depending on the current options this will remove the path of filenames
const char* skipPathFromFilename(const char* file) {
#ifndef DOCTEST_CONFIG_DISABLE
    if(getContextOptions()->no_path_in_filenames) {
        auto back    = std::strrchr(file, '\\');
        auto forward = std::strrchr(file, '/');
        if(back || forward) {
            if(back > forward)
                forward = back;
            return forward + 1;
        }
    } else {
        const auto prefixes = getContextOptions()->strip_file_prefixes;
        const char separator = DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR;
        String::size_type longest_match = 0U;
        for(String::size_type pos = 0U; pos < prefixes.size(); ++pos)
        {
            const auto prefix_start = pos;
            pos = std::min(prefixes.find(separator, prefix_start), prefixes.size());

            const auto prefix_size = pos - prefix_start;
            if(prefix_size > longest_match)
            {
                // TODO under DOCTEST_MSVC: does the comparison need strnicmp() to work with drive letter capitalization?
                if(0 == std::strncmp(prefixes.c_str() + prefix_start, file, prefix_size))
                {
                    longest_match = prefix_size;
                }
            }
        }
        return &file[longest_match];
    }
#endif // DOCTEST_CONFIG_DISABLE
    return file;
}
DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

} // namespace doctest

