#ifndef DOCTEST_PARTS_PUBLIC_ASSERT_MESSAGE
#define DOCTEST_PARTS_PUBLIC_ASSERT_MESSAGE

#include "doctest/parts/public/assert/type.h"
#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {

    struct DOCTEST_INTERFACE MessageData
    {
        String           m_string;
        const char*      m_file;
        int              m_line;
        assertType::Enum m_severity;
    };

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {

    struct DOCTEST_INTERFACE MessageBuilder : public MessageData
    {
        std::ostream* m_stream;
        bool          logged = false;

        MessageBuilder(const char* file, int line, assertType::Enum severity);

        MessageBuilder(const MessageBuilder&) = delete;
        MessageBuilder(MessageBuilder&&) = delete;

        MessageBuilder& operator=(const MessageBuilder&) = delete;
        MessageBuilder& operator=(MessageBuilder&&) = delete;

        ~MessageBuilder();

        // the preferred way of chaining parameters for stringification
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866)
        template <typename T>
        MessageBuilder& operator,(const T& in) {
            *m_stream << (DOCTEST_STRINGIFY(in));
            return *this;
        }
DOCTEST_MSVC_SUPPRESS_WARNING_POP

        // kept here just for backwards-compatibility - the comma operator should be preferred now
        template <typename T>
        MessageBuilder& operator<<(const T& in) { return this->operator,(in); }

        // the `,` operator has the lowest operator precedence - if `<<` is used by the user then
        // the `,` operator will be called last which is not what we want and thus the `*` operator
        // is used first (has higher operator precedence compared to `<<`) so that we guarantee that
        // an operator of the MessageBuilder class is called first before the rest of the parameters
        template <typename T>
        MessageBuilder& operator*(const T& in) { return this->operator,(in); }

        bool log();
        void react();
    };

} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_ASSERT_MESSAGE
