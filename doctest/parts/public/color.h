namespace doctest {
namespace Color {
    enum Enum
    {
        None = 0,
        White,
        Red,
        Green,
        Blue,
        Cyan,
        Yellow,
        Grey,

        Bright = 0x10,

        BrightRed   = Bright | Red,
        BrightGreen = Bright | Green,
        LightGrey   = Bright | Grey,
        BrightWhite = Bright | White
    };

    DOCTEST_INTERFACE std::ostream& operator<<(std::ostream& s, Color::Enum code);
} // namespace Color
}
