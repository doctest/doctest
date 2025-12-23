#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/assert/handler.h"
#include "doctest/parts/private/reporter.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    MessageBuilder::MessageBuilder(const char* file, int line, assertType::Enum severity) {
        m_stream   = tlssPush();
        m_file     = file;
        m_line     = line;
        m_severity = severity;
    }

    MessageBuilder::~MessageBuilder() {
        if (!logged)
            tlssPop();
    }

    bool MessageBuilder::log() {
        if (!logged) {
            m_string = tlssPop();
            logged = true;
        }

        DOCTEST_ITERATE_THROUGH_REPORTERS(log_message, *this);

        const bool isWarn = m_severity & assertType::is_warn;

        // warn is just a message in this context so we don't treat it as an assert
        if(!isWarn) {
            addAssert(m_severity);
            addFailedAssert(m_severity);
        }

        return isDebuggerActive() && !getContextOptions()->no_breaks && !isWarn &&
            (g_cs->currentTest == nullptr || !g_cs->currentTest->m_no_breaks); // break into debugger
    }

    void MessageBuilder::react() {
        if(m_severity & assertType::is_require) //!OCLINT bitwise operator in conditional
            throwException();
    }

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
