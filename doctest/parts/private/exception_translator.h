#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest { namespace detail {

    std::vector<const IExceptionTranslator*>& getExceptionTranslators();
    String                                    translateActiveException();

}} // namespace doctest::detail

#endif // DOCTEST_CONFIG_DISABLE
