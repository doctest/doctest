#ifndef DOCTEST_PARTS_PUBLIC_SUBCASE
#define DOCTEST_PARTS_PUBLIC_SUBCASE

#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {

enum class filterable { off, on };

class DOCTEST_INTERFACE DecoratedName : public String {
public:
    using String::String;
    bool m_filterable = true;
    DecoratedName operator+(const DecoratedName &other) {
        DecoratedName result = *this;
        result += other;
        return result;
    }
    DecoratedName &operator*(filterable f) noexcept {
        m_filterable = (f == filterable::on);
        return *this;
    }
    template <typename T>
    DecoratedName &operator->*(const T &in) {
        String::operator=(in);
        return *this;
    }
};

struct DOCTEST_INTERFACE SubcaseSignature {
    String m_name;
    const char *m_file;
    int m_line;

    bool operator==(const SubcaseSignature &other) const;
    bool operator<(const SubcaseSignature &other) const;
};

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {
struct DOCTEST_INTERFACE Subcase {
    SubcaseSignature m_signature;
    bool m_entered = false;

    Subcase(const DecoratedName &name, const char *file, int line);
    Subcase(const Subcase &) = delete;
    Subcase(Subcase &&) = delete;
    Subcase &operator=(const Subcase &) = delete;
    Subcase &operator=(Subcase &&) = delete;
    ~Subcase();

    operator bool() const;

private:
    bool checkFilters();
};
} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_SUBCASE
