#include <doctest/doctest.h>
#include <utility>

namespace {

/**
 * ExpressionDecomposer sucks to use in isolation.
 * For now, we can use this macro to decompose an expression.
 * Key precondition: the expression must resolve to a truthy value!
 *
 * Explanation of the macro body itself:
 *  - We use CHECK_FALSE to ensure that Expression_lhs<T>::operator Result
 *    sees a `false`, hence believes it needs to actually do decomposition,
 *    Without this it just yields a blank string.
 *  - We cast to `const Result &` because otherwise, we have an `Expression_lhs<T> &&`
 *    which yields a `Result &&`, and this just breaks in fun ways
 *  - We cast `.m_decomp` to `String` simply to ensure we get a copy of it, so we don't dangle
 *
 * In the future, we should just make the Expression API a little nicer to work with,
 * so we can perform these checks a lot more easily.
 */
#define DECOMPOSE(...)                                                                          \
  static_cast<doctest::String>(                                                                 \
    static_cast<const doctest::detail::Result &>(                                               \
      doctest::detail::ExpressionDecomposer(doctest::assertType::DT_CHECK_FALSE) << __VA_ARGS__ \
    ).m_decomp                                                                                  \
  )                                                                                             \

} // namespace

TEST_SUITE("Expression decomposition") {

    // Collection of lvalues / basis for xvalues / prvalues
    const auto _false  = false;
    const auto _true   = true;
    const auto _1      = int { 1 };
    const auto _2      = int { 2 };
    const auto _foo    = doctest::String("foo");
    const auto _bar    = doctest::String("bar");
    const auto _foobar = doctest::String("foobar");

    const int *ptr      = &_1;
    const int  array[2] = { 1, 2 };

    struct { int value: 4; } bitfield = { 1 };

    struct Object { int var = 1; void method() { } };
    const auto member_var    = &Object::var;
    const auto member_method = &Object::method;
    const auto object        = Object();

    enum         { X = 1, Y = 2 };
    enum class C { X = 1, Y = 2 };

    inline void function() { }

    // Generate an xvalue from a prvalue
    template <typename T>
    T &&xvalue(T &&value) { return static_cast<T &&>(value); }


    TEST_CASE("Unary decomposition") {
        SUBCASE("boolean [lvalue]") {
            CHECK(DECOMPOSE(_true) == "true");
        }

        SUBCASE("boolean [prvalue]") {
            CHECK(DECOMPOSE(true) == "true");
        }

        SUBCASE("boolean [xvalue]") {
            CHECK(DECOMPOSE(xvalue(true)) == "true");
        }

        SUBCASE("Array subscript [lvalue]") {
            CHECK(DECOMPOSE(array[0]) == "1");
        }

        SUBCASE("Bitfield member [xvalue]") {
            CHECK(DECOMPOSE(bitfield.value) == "1");
        }

        SUBCASE("Pointer-to-member [lvalue]") {
            CHECK(DECOMPOSE(member_var)    == "{?}");
            CHECK(DECOMPOSE(member_method) == "{?}");
        }

        SUBCASE("Pointer-to-member [prvalue]") {
            CHECK(DECOMPOSE(&Object::var)    == "{?}");
            CHECK(DECOMPOSE(&Object::method) == "{?}");
        }

        SUBCASE("Resolved pointer-to-member [lvalue]") {
            CHECK(DECOMPOSE(object.*member_var) == "1");
        }

        SUBCASE("Unscoped enumerator [prvalue]") {
            CHECK(DECOMPOSE(X) == "1");
        }

        SUBCASE("Scoped enumerator [prvalue]") {
            CHECK(DECOMPOSE(C::X) == "1");
        }

        SUBCASE("Function [lvalue]") {
            CHECK(DECOMPOSE(function) == "{?}");
        }
    }

    TEST_CASE("Binary decomposition") {
        // For a few of these checks, we don't actually have stringification available
        // As a result, a few checks will resolve to {?} == {?} or similar
        // This is fine, since we really just want to make sure that the decomposition
        // is able to accept these values; the stringification is another module's problem.

        SUBCASE("boolean [lvalue]") {
            CHECK(DECOMPOSE(_true == _true)  == "true == true");
            CHECK(DECOMPOSE(_true != _false) == "true != false");
        }

        SUBCASE("boolean [prvalue]") {
            CHECK(DECOMPOSE(true == true)  == "true == true");
            CHECK(DECOMPOSE(true != false) == "true != false");
        }

        SUBCASE("boolean [xvalue]") {
            CHECK(DECOMPOSE(xvalue(true) == xvalue(true))  == "true == true");
            CHECK(DECOMPOSE(xvalue(true) != xvalue(false)) == "true != false");
        }

        SUBCASE("integer [lvalue]") {
            CHECK(DECOMPOSE(_1 == _1) == "1 == 1");
            CHECK(DECOMPOSE(_1 != _2) == "1 != 2");
            CHECK(DECOMPOSE(_1 <  _2) == "1 <  2");
            CHECK(DECOMPOSE(_2 >  _1) == "2 >  1");
            CHECK(DECOMPOSE(_1 <= _2) == "1 <= 2");
            CHECK(DECOMPOSE(_2 >= _1) == "2 >= 1");
        }

        SUBCASE("String [prvalue]") {
            CHECK(DECOMPOSE(_foo + _bar == _foobar) == "foobar == foobar");
        }

        SUBCASE("Pointer dereference [lvalue]") {
            CHECK(DECOMPOSE(*ptr == _1) == "1 == 1");
        }

        SUBCASE("Array subscript [lvalue]") {
            CHECK(DECOMPOSE(array[0] != array[1]) == "1 != 2");
        }

        SUBCASE("Bitfield member [xvalue]") {
            CHECK(DECOMPOSE(bitfield.value == _1) == "1 == 1");
        }

        SUBCASE("Pointer-to-member [lvalue]") {
            CHECK(DECOMPOSE(member_var    == member_var)    == "{?} == {?}");
            CHECK(DECOMPOSE(member_method == member_method) == "{?} == {?}");
        }

        SUBCASE("Pointer-to-member [prvalue]") {
            CHECK(DECOMPOSE(&Object::var    == &Object::var)    == "{?} == {?}");
            CHECK(DECOMPOSE(&Object::method == &Object::method) == "{?} == {?}");
        }

        SUBCASE("Resolved pointer-to-member [lvalue]") {
            CHECK(DECOMPOSE(object.*member_var == _1) == "1 == 1");
        }

        SUBCASE("Unscoped enumerator [prvalue]") {
            CHECK(DECOMPOSE(X != Y) == "1 != 2");
        }

        SUBCASE("Scoped enumerator [prvalue]") {
            CHECK(DECOMPOSE(C::X != C::Y) == "1 != 2");
        }

        SUBCASE("Function [lvalue]") {
            CHECK(DECOMPOSE(function == function) == "{?} == {?}");
        }
    }
}
