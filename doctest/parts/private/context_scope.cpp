#include "doctest/parts/private/context_scope.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/exceptions.h"

#include <sstream>

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {

DOCTEST_DEFINE_INTERFACE(IContextScope)

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {
DOCTEST_THREAD_LOCAL std::vector<IContextScope *> *g_infoContexts = nullptr;

std::vector<IContextScope *> &getInfoContexts() {
    if (g_infoContexts == nullptr)
        g_infoContexts = new std::vector<IContextScope *>;
    return *g_infoContexts;
}

ContextScopeBase::ContextScopeBase() {
    getInfoContexts().push_back(this);
}

ContextScopeBase::ContextScopeBase(ContextScopeBase &&other) noexcept {
    if (other.need_to_destroy) {
        other.destroy();
    }
    other.need_to_destroy = false;
    getInfoContexts().push_back(this);
}

// destroy cannot be inlined into the destructor because that would mean calling stringify after
// ContextScope has been destroyed (base class destructors run after derived class destructors).
// Instead, ContextScope calls this method directly from its destructor.
void ContextScopeBase::destroy() {
    if (detail::has_uncaught_exceptions()) {
        std::ostringstream s;
        this->stringify(&s);
        g_cs->stringifiedContexts.emplace_back(s.str().c_str());
    }
    getInfoContexts().pop_back();
}

} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
