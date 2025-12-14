#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    std::vector<const IExceptionTranslator*>& getExceptionTranslators();
    String translateActiveException();

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
