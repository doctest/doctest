# Change Log

## [1.2.7](https://github.com/onqtam/doctest/tree/1.2.7) (2018-02-06)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.6...1.2.7)

**Closed issues:**

- MSan has runtime error: unsigned integer overflow [\#116](https://github.com/onqtam/doctest/issues/116)
- clang-tidy warning about cert-err58-cpp [\#115](https://github.com/onqtam/doctest/issues/115)
- Linking errors [\#113](https://github.com/onqtam/doctest/issues/113)
- inlining function defs [\#111](https://github.com/onqtam/doctest/issues/111)
- Nullptr issue. [\#110](https://github.com/onqtam/doctest/issues/110)
- MemorySanitizer: use-of-uninitialized-value [\#109](https://github.com/onqtam/doctest/issues/109)
- Potential memory leak through scan-build [\#108](https://github.com/onqtam/doctest/issues/108)
- Warnings raised to error with lastest MSVC version [\#107](https://github.com/onqtam/doctest/issues/107)
- New solution for tests in static libraries ! \(MSVC\) [\#106](https://github.com/onqtam/doctest/issues/106)
- Command line flags do not work after code formatter/beautifier [\#104](https://github.com/onqtam/doctest/issues/104)
- Cppcheck 1.81 warnings [\#102](https://github.com/onqtam/doctest/issues/102)

**Merged pull requests:**

- Fix macros WIN32\_LEAN\_AND\_MEAN typo [\#112](https://github.com/onqtam/doctest/pull/112) ([vladimirgamalyan](https://github.com/vladimirgamalyan))
- Correct DOCTEST\_NO\_INSTALL logic; do install unless it is set \(\#99\) [\#100](https://github.com/onqtam/doctest/pull/100) ([onqtam](https://github.com/onqtam))
- Correct DOCTEST\_NO\_INSTALL logic; do install unless it is set [\#99](https://github.com/onqtam/doctest/pull/99) ([OdyX](https://github.com/OdyX))

## [1.2.6](https://github.com/onqtam/doctest/tree/1.2.6) (2017-10-29)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.5...1.2.6)

**Closed issues:**

- \[bug\] writing an exception translator in a header file results in it being registered multiple times which is suboptimal [\#98](https://github.com/onqtam/doctest/issues/98)
- Warnings when using something more than /W4 for Visual Studio [\#95](https://github.com/onqtam/doctest/issues/95)

**Merged pull requests:**

- Added an option to not install Doctest in the CMake scripts [\#96](https://github.com/onqtam/doctest/pull/96) ([NeverMine17](https://github.com/NeverMine17))
- Adding a defensive check against a null pointer for the current test suite [\#94](https://github.com/onqtam/doctest/pull/94) ([Lectem](https://github.com/Lectem))
- Remove incomplete copy ctor [\#93](https://github.com/onqtam/doctest/pull/93) ([McMartin](https://github.com/McMartin))

## [1.2.5](https://github.com/onqtam/doctest/tree/1.2.5) (2017-10-06)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.4...1.2.5)

**Closed issues:**

- Xcode 9 / clang - unknown warning group [\#92](https://github.com/onqtam/doctest/issues/92)

## [1.2.4](https://github.com/onqtam/doctest/tree/1.2.4) (2017-09-20)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.3...1.2.4)

**Closed issues:**

- \[bug\] test cases can end up in the wrong test suite [\#91](https://github.com/onqtam/doctest/issues/91)

## [1.2.3](https://github.com/onqtam/doctest/tree/1.2.3) (2017-09-11)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.2...1.2.3)

**Closed issues:**

- \[bug\] Defining a variable T inside a test with DOCTEST\_CONFIG\_DISABLE defined does not compile [\#90](https://github.com/onqtam/doctest/issues/90)
- \[support\] Using `DOCTEST\_CONFIG\_NO\_SHORT\_MACRO\_NAMES` does not compile using g++ 6.3.0 [\#89](https://github.com/onqtam/doctest/issues/89)
- \[question\] Why are SUBCASEs executed only once when within a function called multiple times? [\#88](https://github.com/onqtam/doctest/issues/88)

## [1.2.2](https://github.com/onqtam/doctest/tree/1.2.2) (2017-09-05)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.1...1.2.2)

**Closed issues:**

- \[question\] Differences between doctest and googletest \(gtest\) for uninitialised local variables in test cases [\#86](https://github.com/onqtam/doctest/issues/86)
- !!! BREAKING CHANGE !!! - remove the custom implementation of std::is\_constructible and optionally use the \<type\_traits\> header because of infinite template recursion issues with GCC [\#85](https://github.com/onqtam/doctest/issues/85)
- Static Analysis results of doctest [\#83](https://github.com/onqtam/doctest/issues/83)
- !!! BREAKING CHANGE !!! - catch exceptions as const reference in \<LEVEL\>\_THROWS\_AS [\#81](https://github.com/onqtam/doctest/issues/81)
- doctest implementation as static library [\#77](https://github.com/onqtam/doctest/issues/77)
- \[question\] Private members [\#76](https://github.com/onqtam/doctest/issues/76)
- Provide some easy way to compare structs containing float/doubles [\#73](https://github.com/onqtam/doctest/issues/73)

**Merged pull requests:**

- Add support for templated scenarios [\#87](https://github.com/onqtam/doctest/pull/87) ([Lectem](https://github.com/Lectem))
- Prefer if\(MSVC\) in CMakeLists.txt. [\#84](https://github.com/onqtam/doctest/pull/84) ([martinmoene](https://github.com/martinmoene))
- catch throw\_as exception as const reference [\#82](https://github.com/onqtam/doctest/pull/82) ([a4z](https://github.com/a4z))
- Added doctest\_with\_main static lib [\#78](https://github.com/onqtam/doctest/pull/78) ([ymadzhunkov](https://github.com/ymadzhunkov))

## [1.2.1](https://github.com/onqtam/doctest/tree/1.2.1) (2017-05-24)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.0...1.2.1)

**Closed issues:**

- Compile error under MSVC 2015/2017 if \<thread\> included in same file as "doctest.h" [\#72](https://github.com/onqtam/doctest/issues/72)

**Merged pull requests:**

- docs: TEST\_CASE\_METHOD -\> TEST\_CASE\_FIXTURE [\#71](https://github.com/onqtam/doctest/pull/71) ([akrzemi1](https://github.com/akrzemi1))

## [1.2.0](https://github.com/onqtam/doctest/tree/1.2.0) (2017-05-15)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.1.4...1.2.0)

**Closed issues:**

- Further improvements on compile time - disable inlining of functions used in asserts [\#70](https://github.com/onqtam/doctest/issues/70)
- Improve runtime performance - lazy stringification, more inlining, no statics on the hot path, move semantics for classes such as doctest::String which are used by value, etc. [\#69](https://github.com/onqtam/doctest/issues/69)
- Add option to show duration of test case execution and add a timeout\(seconds\) decorator - marking them as failed if they exceed it [\#68](https://github.com/onqtam/doctest/issues/68)
- Add support for test case decorators - label, description, skip, may\_fail, should\_fail, expected\_failures, etc. [\#67](https://github.com/onqtam/doctest/issues/67)
- Integrate static analysis into the CI builds [\#66](https://github.com/onqtam/doctest/issues/66)
- Print the test suite name on test case failure [\#65](https://github.com/onqtam/doctest/issues/65)
- Add signal handlers to handle crashes \(and use SEH under Windows\) - report which test case failed [\#63](https://github.com/onqtam/doctest/issues/63)
- Add support to Approx for strong typedefs of double [\#62](https://github.com/onqtam/doctest/issues/62)
- \[question\] Is there a way to always have 0 as the exit code regardless of test results? [\#59](https://github.com/onqtam/doctest/issues/59)
- Add support for un-parenthesized expressions containing commas in asserts [\#58](https://github.com/onqtam/doctest/issues/58)
- Add ability to filter subcases with filters [\#57](https://github.com/onqtam/doctest/issues/57)
- Add option to query if code is being ran inside of a test - doctest::isRunningInTest\(\) [\#56](https://github.com/onqtam/doctest/issues/56)
- Ability for a binary \(executable / shared object\) to use the test runner implementation of another binary - with exported symbols - so tests end up in a single registry [\#55](https://github.com/onqtam/doctest/issues/55)
- How to force the use of colors in the terminal? [\#54](https://github.com/onqtam/doctest/issues/54)
- How can I mix production code with the Unit Tests? [\#53](https://github.com/onqtam/doctest/issues/53)
- add \<= and \>= operators to Approx \(and also maybe \< and \>\) [\#52](https://github.com/onqtam/doctest/issues/52)
- Add ability to capture variables from test scope [\#48](https://github.com/onqtam/doctest/issues/48)
- !!! BREAKING CHANGE !!! - Make TEST\_SUITE work with blocks and add TEST\_SUITE\_BEGIN [\#41](https://github.com/onqtam/doctest/issues/41)
- Add option to print which test suites/cases are run [\#39](https://github.com/onqtam/doctest/issues/39)
- Add support for templated test cases - parameterized by type [\#38](https://github.com/onqtam/doctest/issues/38)
- Add custom failure messages with lazy stringification [\#23](https://github.com/onqtam/doctest/issues/23)
- Add an exception translation mechanism + the ability for users to extend it with custom exception types [\#12](https://github.com/onqtam/doctest/issues/12)
- Add API for reporting failures [\#9](https://github.com/onqtam/doctest/issues/9)

**Merged pull requests:**

- Update doctest to work with ARM DS5-compiler [\#64](https://github.com/onqtam/doctest/pull/64) ([tomasnilefrost](https://github.com/tomasnilefrost))

## [1.1.4](https://github.com/onqtam/doctest/tree/1.1.4) (2017-02-18)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.1.3...1.1.4)

**Closed issues:**

- Add option --force-colors - for when a tty is not detected for stdout [\#51](https://github.com/onqtam/doctest/issues/51)
- Issue with using lambdas in tests in gcc [\#49](https://github.com/onqtam/doctest/issues/49)
- Add the include file to releases [\#47](https://github.com/onqtam/doctest/issues/47)

**Merged pull requests:**

- Add translation of std::exception for exceptions that terminate a test case [\#46](https://github.com/onqtam/doctest/pull/46) ([eliaskosunen](https://github.com/eliaskosunen))

## [1.1.3](https://github.com/onqtam/doctest/tree/1.1.3) (2016-11-15)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.1.2...1.1.3)

**Closed issues:**

- Exception handlers cause warnings when exceptions are disabled [\#44](https://github.com/onqtam/doctest/issues/44)

## [1.1.2](https://github.com/onqtam/doctest/tree/1.1.2) (2016-10-10)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.1.1...1.1.2)

**Closed issues:**

- clang warnings when using C++11 or newer [\#42](https://github.com/onqtam/doctest/issues/42)
- \[support\] identical names for test suites? [\#40](https://github.com/onqtam/doctest/issues/40)

## [1.1.1](https://github.com/onqtam/doctest/tree/1.1.1) (2016-09-22)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.1.0...1.1.1)

## [1.1.0](https://github.com/onqtam/doctest/tree/1.1.0) (2016-09-21)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.0.0...1.1.0)

**Closed issues:**

- char\* comparison uses the contents, not the pointer [\#36](https://github.com/onqtam/doctest/issues/36)
- add configuration preprocessor identifier for passing by value in assertions instead of by reference [\#35](https://github.com/onqtam/doctest/issues/35)
- restrict expressions in assertion macros to binary comparisons at most with a static assert [\#34](https://github.com/onqtam/doctest/issues/34)
- Add clearFilters\(\) to doctest::Context [\#33](https://github.com/onqtam/doctest/issues/33)
- A way to refrain from polluting “\#define” space for users of tested code? [\#32](https://github.com/onqtam/doctest/issues/32)
- drop VC++6 support [\#31](https://github.com/onqtam/doctest/issues/31)
- False positive test [\#30](https://github.com/onqtam/doctest/issues/30)
- Turn off coloring after tests are finished? [\#28](https://github.com/onqtam/doctest/issues/28)
- C++11 nullptr [\#27](https://github.com/onqtam/doctest/issues/27)
- Only one SUBCASE per line is executed [\#25](https://github.com/onqtam/doctest/issues/25)
- creative formatting of chars [\#24](https://github.com/onqtam/doctest/issues/24)
- DOCTEST\_BREAK\_INTO\_DEBUGGER undefined under OSX [\#22](https://github.com/onqtam/doctest/issues/22)
- Tests inside a static library [\#21](https://github.com/onqtam/doctest/issues/21)
- Add example how to remove doctest options from the command line for the program after the tests run [\#20](https://github.com/onqtam/doctest/issues/20)
- Single-letter options active even without leading '-' \(dash\) [\#19](https://github.com/onqtam/doctest/issues/19)
- pointer stringification not working for compilers different from MSVC [\#18](https://github.com/onqtam/doctest/issues/18)
- Tests that accompany code run and produce output at default [\#17](https://github.com/onqtam/doctest/issues/17)
- GCC 5.3.1 Compiler warning: sign compare [\#16](https://github.com/onqtam/doctest/issues/16)
- Slower than Catch in realistic test cases [\#14](https://github.com/onqtam/doctest/issues/14)
- Rename doctest::detail::Result res; in DOCTEST\_ASSERT\_IMPLEMENT [\#10](https://github.com/onqtam/doctest/issues/10)
- No red when all tests pass [\#7](https://github.com/onqtam/doctest/issues/7)
- UNIX line feedings on GitHub please [\#6](https://github.com/onqtam/doctest/issues/6)

**Merged pull requests:**

- dont show green when tests fail [\#26](https://github.com/onqtam/doctest/pull/26) ([ferkulat](https://github.com/ferkulat))
- Include "program code" in example [\#15](https://github.com/onqtam/doctest/pull/15) ([martinmoene](https://github.com/martinmoene))

## [1.0.0](https://github.com/onqtam/doctest/tree/1.0.0) (2016-05-22)
**Merged pull requests:**

- Reduce the header size for test users [\#3](https://github.com/onqtam/doctest/pull/3) ([zah](https://github.com/zah))
- Add a Gitter chat badge to README.md [\#1](https://github.com/onqtam/doctest/pull/1) ([gitter-badger](https://github.com/gitter-badger))



\* *This Change Log was automatically generated by [github_changelog_generator](https://github.com/skywinder/Github-Changelog-Generator)*