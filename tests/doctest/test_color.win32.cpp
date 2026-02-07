#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <utils/callback.h>

#if defined(DOCTEST_CONFIG_COLORS_WINDOWS)

namespace {

class callbacks {
private:
    static callbacks *_instance;

public:
    callbacks() {
        _instance = this;
    }

    ~callbacks() {
        _instance = nullptr;
    }

    static callbacks *instance() {
        return _instance;
    }

#define DEFINE_CALLBACK(_fn) \
    utils::callback<decltype(_fn)> _fn##_

    DEFINE_CALLBACK(GetStdHandle);
    DEFINE_CALLBACK(GetConsoleScreenBufferInfo);
    DEFINE_CALLBACK(SetConsoleTextAttribute);
};

callbacks *callbacks::_instance = nullptr;

} // namespace


extern "C" {

DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Winconsistent-dllimport")
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wattributes")
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4273)
HANDLE WINAPI GetStdHandle(_In_ DWORD nStdHandle) {
    auto *self = callbacks::instance();
    return self->GetStdHandle_(nStdHandle);
}

BOOL WINAPI GetConsoleScreenBufferInfo(_In_  HANDLE                      hConsoleOutput,
                                       _Out_ PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo) {
    auto *self = callbacks::instance();
    return self->GetConsoleScreenBufferInfo_(hConsoleOutput, lpConsoleScreenBufferInfo);
}

BOOL WINAPI SetConsoleTextAttribute(_In_ HANDLE hConsoleOutput,
                                    _In_ WORD   wAttributes) {
    auto *self = callbacks::instance();
    return self->SetConsoleTextAttribute_(hConsoleOutput, wAttributes);
}
DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP

} // extern "C"


TEST_CASE_FIXTURE(callbacks, "Colorizing a stream") {
    using namespace doctest;

    // We can't actually test this more than once,
    // since the method internally stores a `static struct ConsoleHelper`
    // object to memoize the calls. So, we can only one-shot it.

    const auto set_colour = [&](doctest::Color::Enum color) {
        std::ostringstream oss { };
        doctest::detail::g_cs->force_colors = true;
        oss << color;
        doctest::detail::g_cs->force_colors = false;

        // This code path doesn't actually write the stream,
        // so instead return the expected call from SetConsoleTextAttribute
        return SetConsoleTextAttribute_.latest_call();
    };
    using result = std::tuple<HANDLE, WORD>;

    const auto handle = reinterpret_cast<HANDLE>(42);
    GetStdHandle_ = [&](_In_ DWORD) { return handle; };

    const auto fg = WORD { };
    const auto bg = WORD { BACKGROUND_INTENSITY | BACKGROUND_RED };
    GetConsoleScreenBufferInfo_ = [&](_In_ HANDLE, _Out_ PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo) {
        lpConsoleScreenBufferInfo->wAttributes = static_cast<WORD>(fg | bg);
        return TRUE;
    };

    CHECK(set_colour(Color::None) == result {
        handle, bg
    });

    CHECK(set_colour(Color::White) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
    });

    CHECK(set_colour(Color::Red) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_RED)
    });

    CHECK(set_colour(Color::Green) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_GREEN)
    });

    CHECK(set_colour(Color::Blue) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_BLUE)
    });

    CHECK(set_colour(Color::Cyan) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_GREEN | FOREGROUND_BLUE)
    });

    CHECK(set_colour(Color::Yellow) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_RED | FOREGROUND_GREEN)
    });

    CHECK(set_colour(Color::Grey) == result {
        handle, bg
    });

    CHECK(set_colour(Color::BrightRed) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_INTENSITY | FOREGROUND_RED)
    });

    CHECK(set_colour(Color::BrightGreen) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_INTENSITY | FOREGROUND_GREEN)
    });

    CHECK(set_colour(Color::LightGrey) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_INTENSITY)
    });

    CHECK(set_colour(Color::BrightWhite) == result {
        handle, static_cast<WORD>(bg | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
    });
}

#endif // defined(DOCTEST_CONFIG_COLORS_WINDOWS)
