#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <doctest/doctest.h>

namespace {
class DummyContextOptions {
private:
    doctest::ContextOptions _old_options;

public:
    doctest::ContextOptions *options;

    inline DummyContextOptions() {
        // Safe since this function is backed by g_cs, which is mutable
        options = const_cast<doctest::ContextOptions *>(doctest::getContextOptions());

        // Retain a copy
        _old_options = *options;
    }

    inline ~DummyContextOptions() {
        *options = _old_options;
    }
};
} // namespace

TEST_CASE("Determining basename from path") {
    using doctest::skipPathFromFilename;
    DummyContextOptions context { };

    SUBCASE("With no_path_in_filenames enabled") {
        context.options->no_path_in_filenames = true;

        CHECK(skipPathFromFilename("")                          == "");
        CHECK(skipPathFromFilename("file.c")                    == "file.c");
        CHECK(skipPathFromFilename("path/to/file.c")            == "file.c");
        CHECK(skipPathFromFilename("path\\to\\file.c")          == "file.c");
        CHECK(skipPathFromFilename("another/path/to/file.c")    == "file.c");
        CHECK(skipPathFromFilename("another\\path\\to\\file.c") == "file.c");
    }

    SUBCASE("With no_path_in_filenames disabled, and strip_file_prefixes empty") {
        context.options->no_path_in_filenames = false;
        context.options->strip_file_prefixes  = "";

        CHECK(skipPathFromFilename("")                          == "");
        CHECK(skipPathFromFilename("file.c")                    == "file.c");
        CHECK(skipPathFromFilename("path/to/file.c")            == "path/to/file.c");
        CHECK(skipPathFromFilename("path\\to\\file.c")          == "path\\to\\file.c");
        CHECK(skipPathFromFilename("another/path/to/file.c")    == "another/path/to/file.c");
        CHECK(skipPathFromFilename("another\\path\\to\\file.c") == "another\\path\\to\\file.c");
    }

    SUBCASE("With no_path_in_filenames disabled, and strip_file_prefixes set to 'path'") {
        context.options->no_path_in_filenames = false;
        context.options->strip_file_prefixes  = "path";

        CHECK(skipPathFromFilename("")                          == "");
        CHECK(skipPathFromFilename("file.c")                    == "file.c");
        CHECK(skipPathFromFilename("path/to/file.c")            == "/to/file.c");
        CHECK(skipPathFromFilename("path\\to\\file.c")          == "\\to\\file.c");
        CHECK(skipPathFromFilename("another/path/to/file.c")    == "another/path/to/file.c");
        CHECK(skipPathFromFilename("another\\path\\to\\file.c") == "another\\path\\to\\file.c");
    }
}
