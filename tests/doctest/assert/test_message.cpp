#include <doctest/doctest.h>
#include <cstdint>

TEST_CASE("Building messages") {
    doctest::detail::MessageBuilder mb("file.c", 123, doctest::assertType::DT_CHECK);

    const auto lhs = uint32_t { 42 };
    const auto rhs = uint32_t { 24 };
    mb * "ERROR: ", lhs, " == ", rhs, " was FALSE";

    // String is generated as part of MessageBuilder::log,
    // but calling this interacts with the overall test
    // So, below is the relevant part of ::log that generates the string
    mb.logged = true;
    mb.m_string = doctest::detail::tlssPop();

    CHECK(mb.m_string   == "ERROR: 42 == 24 was FALSE");
    CHECK(mb.m_file     == "file.c");
    CHECK(mb.m_line     == 123);
    CHECK(mb.m_severity == doctest::assertType::DT_CHECK);
}
