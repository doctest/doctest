namespace doctest {

struct DOCTEST_INTERFACE SubcaseSignature
{
    String      m_name;
    const char* m_file;
    int         m_line;

    bool operator==(const SubcaseSignature& other) const;
    bool operator<(const SubcaseSignature& other) const;
};

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {
struct DOCTEST_INTERFACE Subcase
{
    SubcaseSignature m_signature;
    bool             m_entered = false;

    Subcase(const String& name, const char* file, int line);
    Subcase(const Subcase&) = delete;
    Subcase(Subcase&&) = delete;
    Subcase& operator=(const Subcase&) = delete;
    Subcase& operator=(Subcase&&) = delete;
    ~Subcase();

    operator bool() const;

    private:
        bool checkFilters();
};
} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest
