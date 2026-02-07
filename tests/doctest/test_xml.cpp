#include <doctest/parts/private/xml.h>

#include <sstream>
#include <string>

#include <utils/text.h>
using namespace utils;

DOCTEST_SUPPRESS_COMMON_WARNINGS_PUSH

namespace {

struct XmlWriter {
    std::ostringstream oss { };
    doctest::detail::XmlWriter xml { oss };

    std::string text() const noexcept {
        return oss.str();
    }
};

}

TEST_CASE_FIXTURE(XmlWriter, "Writing XML document declaration") {
    xml.writeDeclaration();
    CHECK(text() == text::dedent(R"(
        <?xml version="1.0" encoding="UTF-8"?>
    )"));
}

TEST_CASE_FIXTURE(XmlWriter, "Writing an empty element") {
    xml.startElement("test")
       .endElement();

    CHECK(text() == text::dedent(R"(
        <test/>
    )"));
}

TEST_CASE_FIXTURE(XmlWriter, "Writing an element with attributes") {
    SUBCASE("Boolean arguments") {
        xml.startElement("test")
           .writeAttribute("x", false)
           .writeAttribute("y", true)
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test x="false" y="true"/>
        )"));
    }

    SUBCASE("String arguments") {
        xml.startElement("test")
           .writeAttribute("non-empty", "data")
           .writeAttribute("empty",     "")
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test non-empty="data"/>
        )"));
    }

    SUBCASE("Stringifiable arguments") {
        xml.startElement("test")
           .writeAttribute("int",  42)
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test int="42"/>
        )"));
    }

    SUBCASE("Empty name") {
        xml.startElement("test")
           .writeAttribute("", 42)
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test/>
        )"));
    }
}

TEST_CASE_FIXTURE(XmlWriter, "Writing an element with internal data") {
    SUBCASE("One line of data") {
        xml.startElement("test")
           .writeText("data")
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test>
              data
            </test>
        )"));
    }

    SUBCASE("Multiple lines of data") {
        xml.startElement("test")
           .writeText("foo")
           .writeText("bar")
           .writeText("baz")
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test>
              foobarbaz
            </test>
        )"));
    }

    SUBCASE("Data with internal newlines") {
        xml.startElement("test")
           .writeText("foo\nbar\nbaz")
           .endElement();

        CHECK(text() == text::dedent(R"(
            <test>
              foo
            bar
            baz
            </test>
        )"));
    }
}

TEST_CASE_FIXTURE(XmlWriter, "Writing a nested structure") {
    xml.startElement("foo")
       .startElement("bar")
       .startElement("baz")
           .writeText("data")
       .endElement()
       .endElement()
       .endElement();

    CHECK(text() == text::dedent(R"(
        <foo>
          <bar>
            <baz>
              data
            </baz>
          </bar>
        </foo>
    )"));
}

TEST_CASE("Explicitly dropping an XmlWriter with unclosed elements") {
    std::ostringstream oss { };
    /* scope */ {
        doctest::detail::XmlWriter xml { oss };
        xml.startElement("foo").startElement("bar").startElement("baz");
    }

    CHECK(oss.str() == text::dedent(R"(
        <foo>
          <bar>
            <baz/>
          </bar>
        </foo>
    )"));
}

TEST_CASE("Escaping special characters") {
    using doctest::detail::XmlEncode;

    struct {
        std::string operator()(const char *str, XmlEncode::ForWhat what = XmlEncode::ForWhat::ForTextNodes) {
            std::ostringstream oss { };
            oss << doctest::detail::XmlEncode(str, what);
            return oss.str();
        }
    } escape;

    SUBCASE("Non-printable ASCII chars") {
        CHECK(escape("\x01") == R"(\x01)");
        CHECK(escape("\x08") == R"(\x08)");
        CHECK(escape("\x0E") == R"(\x0E)");
        CHECK(escape("\x1F") == R"(\x1F)");
        CHECK(escape("\x7F") == R"(\x7F)");
    }

    SUBCASE("Angle-braces") {
        CHECK(escape("<tag>") == "&lt;tag>");

        CHECK(escape("<![CDATA[ ... ]]>") == "&lt;![CDATA[ ... ]]&gt;");
    }

    SUBCASE("Ampersands (used in XML escapes)") {
        CHECK(escape("&") == "&amp;");
    }

    SUBCASE("Single and double-quotes") {
        CHECK(escape("'string'") == "'string'");

        CHECK(escape("\"string\"", XmlEncode::ForWhat::ForAttributes) == "&quot;string&quot;");
        CHECK(escape("\"string\"", XmlEncode::ForWhat::ForTextNodes)  == "\"string\"");
    }

    SUBCASE("Printable ASCII chars") {
        CHECK(escape("data")  == "data");
        CHECK(escape("x y z") == "x y z");
    }

    SUBCASE("Valid UTF-8 sequences") {
        CHECK(escape("\xC3\x9C")         == "\xC3\x9C");         // U+00DC: Latin Capital Letter U with Diaresis
        CHECK(escape("\xEE\x83\x84")     == "\xEE\x83\x84");     // U+30C3: Katakana Letter Small Tu
        CHECK(escape("\xF0\x9F\x98\x8A") == "\xF0\x9F\x98\x8A"); // U+1F60A Smiling Face with Smiling Eyes
    }

    SUBCASE("Invalid UTF-8 sequences") {
        // UTF-8 stores length information in the upper bits of the lead byte
        // If this is not present (i.e. a continuation byte is present instead),
        // it cannot be a valid UTF-8 sequence
        CHECK(escape("\x80") == R"(\x80)");

        // Similarly, if we get a lead byte indicating 2 bytes of data total,
        // but don't actually have 2 bytes, that can't be valid either
        CHECK(escape("\xC2") == R"(\xC2)");

        // After the lead byte, the next byte(s) are continuations which lead with 0b10
        CHECK(escape("\xC2\x7F") == R"(\xC2\x7F)");

        // We can hypothetically encode small codepoints as-if they were larger
        // by simply adding `0b10'000000` sequences; this shouldn't be valid though
        CHECK(escape("\xC0\xAF")         == R"(\xC0\xAF)");
        CHECK(escape("\xE0\x80\xAF")     == R"(\xE0\x80\xAF)");
        CHECK(escape("\xF0\x80\x80\xAF") == R"(\xF0\x80\x80\xAF)");

        // A 4-byte UTF-8 sequence can represent up to (3+6+6+6 = 21) bits of data,
        // but Unicode only currently maps up to U+10FFFF
        // So, trying to encode U+110000 would be invalid
        CHECK(escape("\xF4\x90\x80\x80") == R"(\xF4\x90\x80\x80)");
    }
}

DOCTEST_SUPPRESS_COMMON_WARNINGS_POP
