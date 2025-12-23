#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_OPTIONS_PREFIX
#define DOCTEST_CONFIG_OPTIONS_PREFIX "dt-"
#endif

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {

    void fulltext_log_assert_to_stream(std::ostream& s, const AssertData& rb) {
        if((rb.m_at & (assertType::is_throws_as | assertType::is_throws_with)) ==
            0) //!OCLINT bitwise operator in conditional
            s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << " ) "
                << Color::None;

        if(rb.m_at & assertType::is_throws) { //!OCLINT bitwise operator in conditional
            s << (rb.m_threw ? "threw as expected!" : "did NOT throw at all!") << "\n";
        } else if((rb.m_at & assertType::is_throws_as) &&
                    (rb.m_at & assertType::is_throws_with)) { //!OCLINT
            s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << ", \""
                << rb.m_exception_string.c_str()
                << "\", " << rb.m_exception_type << " ) " << Color::None;
            if(rb.m_threw) {
                if(!rb.m_failed) {
                    s << "threw as expected!\n";
                } else {
                    s << "threw a DIFFERENT exception! (contents: " << rb.m_exception << ")\n";
                }
            } else {
                s << "did NOT throw at all!\n";
            }
        } else if(rb.m_at &
                    assertType::is_throws_as) { //!OCLINT bitwise operator in conditional
            s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << ", "
                << rb.m_exception_type << " ) " << Color::None
                << (rb.m_threw ? (rb.m_threw_as ? "threw as expected!" :
                                                "threw a DIFFERENT exception: ") :
                                "did NOT throw at all!")
                << Color::Cyan << rb.m_exception << "\n";
        } else if(rb.m_at &
                    assertType::is_throws_with) { //!OCLINT bitwise operator in conditional
            s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << ", \""
                << rb.m_exception_string.c_str()
                << "\" ) " << Color::None
                << (rb.m_threw ? (!rb.m_failed ? "threw as expected!" :
                                                "threw a DIFFERENT exception: ") :
                                "did NOT throw at all!")
                << Color::Cyan << rb.m_exception << "\n";
        } else if(rb.m_at & assertType::is_nothrow) { //!OCLINT bitwise operator in conditional
            s << (rb.m_threw ? "THREW exception: " : "didn't throw!") << Color::Cyan
                << rb.m_exception << "\n";
        } else {
            s << (rb.m_threw ? "THREW exception: " :
                                (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n"));
            if(rb.m_threw)
                s << rb.m_exception << "\n";
            else
                s << "  values: " << assertString(rb.m_at) << "( " << rb.m_decomp << " )\n";
        }
    }

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
