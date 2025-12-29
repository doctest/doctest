#include "doctest/parts/private/prelude.h"

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4007) // 'function' : must be 'attribute' - see issue #182
int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
DOCTEST_MSVC_SUPPRESS_WARNING_POP
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
