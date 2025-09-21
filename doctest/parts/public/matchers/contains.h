namespace doctest {

class DOCTEST_INTERFACE Contains {
public:
    explicit Contains(const String& string);

    bool checkWith(const String& other) const;

    String string;
};

DOCTEST_INTERFACE String toString(const Contains& in);

DOCTEST_INTERFACE bool operator==(const String& lhs, const Contains& rhs);
DOCTEST_INTERFACE bool operator==(const Contains& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator!=(const String& lhs, const Contains& rhs);
DOCTEST_INTERFACE bool operator!=(const Contains& lhs, const String& rhs);

} // namespace doctest
