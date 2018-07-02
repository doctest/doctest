#include "parts/doctest_impl.h"

static const char* getSuccessOrFailString(doctest::assertType::Enum at) {
    using namespace doctest;
    if(at & assertType::is_warn) //!OCLINT bitwise operator in conditional
        return "WARNING: ";
    if(at & assertType::is_check) //!OCLINT bitwise operator in conditional
        return "ERROR: ";
    if(at & assertType::is_require) //!OCLINT bitwise operator in conditional
        return "FATAL ERROR: ";
    return "";
}

static void handler(const doctest::AssertData& ad) {
    using namespace doctest;

    auto& s  = std::cout;
    auto& rb = ad;

    s << Color::LightGrey << ad.m_file << (detail::getContextOptions()->gnu_file_line ? ":" : "(")
      << ad.m_line << (detail::getContextOptions()->gnu_file_line ? ":" : "): ");
    s << Color::Red << getSuccessOrFailString(ad.m_at);

    if((rb.m_at & assertType::is_throws_as) == 0) //!OCLINT bitwise operator in conditional
        s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << " ) " << Color::None;

    if(rb.m_at & assertType::is_throws) { //!OCLINT bitwise operator in conditional
        s << (rb.m_threw ? "threw as expected!" : "did NOT throw at all!") << "\n";
    } else if(rb.m_at & assertType::is_throws_as) { //!OCLINT bitwise operator in conditional
        s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << ", "
          << rb.m_exception_type << " ) " << Color::None
          << (rb.m_threw ?
                      (rb.m_threw_as ? "threw as expected!" : "threw a DIFFERENT exception: ") :
                      "did NOT throw at all!")
          << Color::Cyan << rb.m_exception << "\n";
    } else if(rb.m_at & assertType::is_nothrow) { //!OCLINT bitwise operator in conditional
        s << (rb.m_threw ? "THREW exception: " : "didn't throw!") << Color::Cyan << rb.m_exception
          << "\n";
    } else {
        s << (rb.m_threw ? "THREW exception: " :
                           (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n"));
        if(rb.m_threw)
            s << rb.m_exception << "\n";
        else
            s << "  values: " << assertString(rb.m_at) << "( " << rb.m_decomposition << " )\n";
    }
}

int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in the name
    context.setOption("no-breaks", true); // don't break in the debugger when assertions fail

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("order-by", "file"); // sort the test cases by their name

    // required so asserts can be used outside of a 'run' context
    context.setAsDefaultForAssertsOutOfTestCases();
    context.setAssertHandler(handler);
    void some_func();
    some_func();

    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

#ifdef WITH_PAUSE
    system("pause");
#endif // WITH_PAUSE

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
