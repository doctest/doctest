#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/reporter.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/context_scope.h"

namespace doctest {
#ifdef DOCTEST_CONFIG_DISABLE

    int                         IReporter::get_num_active_contexts() { return 0; }
    const IContextScope* const* IReporter::get_active_contexts() { return nullptr; }
    int                         IReporter::get_num_stringified_contexts() { return 0; }
    const String*               IReporter::get_stringified_contexts() { return nullptr; }

    int registerReporter(const char*, int, IReporter*) { return 0; }

#else

    DOCTEST_DEFINE_INTERFACE(IReporter)

    int IReporter::get_num_active_contexts() { return detail::g_infoContexts.size(); }
    const IContextScope* const* IReporter::get_active_contexts() {
        return get_num_active_contexts() ? &detail::g_infoContexts[0] : nullptr;
    }

    int IReporter::get_num_stringified_contexts() { return detail::g_cs->stringifiedContexts.size(); }
    const String* IReporter::get_stringified_contexts() {
        return get_num_stringified_contexts() ? &detail::g_cs->stringifiedContexts[0] : nullptr;
    }

    namespace detail {
        void registerReporterImpl(const char* name, int priority, reporterCreatorFunc c, bool isReporter) {
            if(isReporter)
                getReporters().insert(reporterMap::value_type(reporterMap::key_type(priority, name), c));
            else
                getListeners().insert(reporterMap::value_type(reporterMap::key_type(priority, name), c));
        }
    } // namespace detail

#endif // DOCTEST_CONFIG_DISABLE
} // namespace doctest
