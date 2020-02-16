# Change Log

## [2.3.6](https://github.com/onqtam/doctest/tree/2.3.6) (2019-12-17)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.3.5...2.3.6)

**Closed issues:**

- \[Feature\] Better integration with tools \(VS Code Test Adapter Extension\) [\#320](https://github.com/onqtam/doctest/issues/320)
- Link problem w/ BUILD=Release if MESSAGE\(\) with std::string/ostream-operator is used [\#316](https://github.com/onqtam/doctest/issues/316)
- the FAQ about difference to Catch2 is missing tags [\#315](https://github.com/onqtam/doctest/issues/315)
- include Windows.h in small caps to silence clang warnings [\#312](https://github.com/onqtam/doctest/issues/312)
- Mistake in generator with lgtm error [\#311](https://github.com/onqtam/doctest/issues/311)
- CMake: cannot install target doctest\_with\_main [\#310](https://github.com/onqtam/doctest/issues/310)
- \[bug\] INFO\(\) and CAPTURE\(\) cannot compile using MSVC when used with DOCTEST\_CONFIG\_IMPLEMENTATION\_IN\_DLL [\#306](https://github.com/onqtam/doctest/issues/306)
- Skip subcase [\#304](https://github.com/onqtam/doctest/issues/304)
- Does some equivalent features from google test exist here? [\#300](https://github.com/onqtam/doctest/issues/300)
- How to use doctest in dll only\(without main.cpp and .exe\) [\#299](https://github.com/onqtam/doctest/issues/299)
- Warning: C26812: The enum type 'doctest::assertType::Enum' is unscoped. Prefer 'enum class' over 'enum' \(Enum.3\). [\#298](https://github.com/onqtam/doctest/issues/298)
- test executable\_dll\_and\_plugin fails on Linux, GCC 8.1.0, -fsanitize=address [\#201](https://github.com/onqtam/doctest/issues/201)

**Merged pull requests:**

- Fixed missing ostream include for MacOS when defining DOCTEST\_CONFIG\_… [\#314](https://github.com/onqtam/doctest/pull/314) ([NKTomHaygarth](https://github.com/NKTomHaygarth))
- include windows.h in cmall caps to silence clang nonportable warnings [\#313](https://github.com/onqtam/doctest/pull/313) ([suoniq](https://github.com/suoniq))
- Add .editorconfig file. [\#301](https://github.com/onqtam/doctest/pull/301) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Add Github Actions CI [\#285](https://github.com/onqtam/doctest/pull/285) ([DaanDeMeyer](https://github.com/DaanDeMeyer))

## [2.3.5](https://github.com/onqtam/doctest/tree/2.3.5) (2019-09-22)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.3.4...2.3.5)

**Closed issues:**

- \[feature request\] Assertion macros for throwing exception of a specific type with message - \<LEVEL\>\_THROWS\_WITH\_AS\(expr, string, ex\_type\) [\#295](https://github.com/onqtam/doctest/issues/295)
- CHECK\_THROWS\_AS of non-default constructor wants to call default constructor [\#293](https://github.com/onqtam/doctest/issues/293)
- Typos and spelling errors in source, documentation and scripts [\#291](https://github.com/onqtam/doctest/issues/291)
- Customize test names / variable substitution [\#284](https://github.com/onqtam/doctest/issues/284)
- SUBCASE in function not behaving as expected [\#282](https://github.com/onqtam/doctest/issues/282)
- SUPER\_FAST\_ASSERTS fails to compile CHECK\_MESSAGE [\#281](https://github.com/onqtam/doctest/issues/281)
- CHECK\_MESSAGE no longer works with DOCTEST\_CONFIG\_SUPER\_FAST\_ASSERTS [\#280](https://github.com/onqtam/doctest/issues/280)
- CAPTURE of structured binding element no longer works [\#279](https://github.com/onqtam/doctest/issues/279)
- Reporter: `test\_case\_end` no longer fired after test case restart [\#278](https://github.com/onqtam/doctest/issues/278)
- Add debug break override support [\#277](https://github.com/onqtam/doctest/issues/277)
- Running tests from within Visual Studio in a static lib project [\#275](https://github.com/onqtam/doctest/issues/275)
- Compile-time error when using a raw string literal inside of REQUIRE \(MSVC 2017\) [\#274](https://github.com/onqtam/doctest/issues/274)
- Give example for having tests in production code [\#252](https://github.com/onqtam/doctest/issues/252)
- Memory leaks just by including doctest.h [\#205](https://github.com/onqtam/doctest/issues/205)
- Feature request: print subcase when an exception is thrown inside one [\#136](https://github.com/onqtam/doctest/issues/136)

**Merged pull requests:**

- Fix typos and misspellings found by codespell. [\#292](https://github.com/onqtam/doctest/pull/292) ([warmsocks](https://github.com/warmsocks))
- Document order by issue correctly [\#290](https://github.com/onqtam/doctest/pull/290) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Document that -order-by=file is compiler-dependent [\#289](https://github.com/onqtam/doctest/pull/289) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Add -order-by=name to filter\_2 test [\#288](https://github.com/onqtam/doctest/pull/288) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Add support for compiling with clang-cl [\#286](https://github.com/onqtam/doctest/pull/286) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- No minimum version limitation of Meson [\#283](https://github.com/onqtam/doctest/pull/283) ([ydm](https://github.com/ydm))

## [2.3.4](https://github.com/onqtam/doctest/tree/2.3.4) (2019-08-12)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.3.3...2.3.4)

**Closed issues:**

- Remove INFO\(\) limitation for using only lvalues and no rvalues [\#269](https://github.com/onqtam/doctest/issues/269)
- Compile error on MAC OS with AppleClang 8.0.0.8000042  [\#266](https://github.com/onqtam/doctest/issues/266)
- Throwing exception in a mocked method [\#265](https://github.com/onqtam/doctest/issues/265)
- Illegal syntax for decorators compiles and runs without warning, but has no effect [\#264](https://github.com/onqtam/doctest/issues/264)
- Support conditional expressions in REQUIRE [\#262](https://github.com/onqtam/doctest/issues/262)
- Register a listener\(reporter\) that always listens [\#257](https://github.com/onqtam/doctest/issues/257)
- Memory sanitizer complaint [\#255](https://github.com/onqtam/doctest/issues/255)
- Windows Clang GNU command line warnings [\#253](https://github.com/onqtam/doctest/issues/253)
- The build writes into the source directory [\#249](https://github.com/onqtam/doctest/issues/249)
- How to enable tests inside another exe [\#246](https://github.com/onqtam/doctest/issues/246)
- Testing multiple headers. [\#244](https://github.com/onqtam/doctest/issues/244)
- CMakeLists.txt: Needs CMAKE\_CXX\_STANDARD=11 [\#243](https://github.com/onqtam/doctest/issues/243)
- \[bug\] Can't compile the tests because of mutex, that is declared in the doctest [\#242](https://github.com/onqtam/doctest/issues/242)

**Merged pull requests:**

- Improve Listener docs [\#273](https://github.com/onqtam/doctest/pull/273) ([claremacrae](https://github.com/claremacrae))
- Rework `INFO` lazy evaluation to use lambdas. [\#270](https://github.com/onqtam/doctest/pull/270) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Prevent compile errors with AppleClang compiler [\#268](https://github.com/onqtam/doctest/pull/268) ([ClausKlein](https://github.com/ClausKlein))
- Revert "fix : includeing windows.h header caause error" [\#263](https://github.com/onqtam/doctest/pull/263) ([onqtam](https://github.com/onqtam))
- Fix static analyzer URLs [\#259](https://github.com/onqtam/doctest/pull/259) ([godbyk](https://github.com/godbyk))
- fix : includeing windows.h header caause error [\#258](https://github.com/onqtam/doctest/pull/258) ([rinechran](https://github.com/rinechran))
- only look for C++ compiler with CMake [\#256](https://github.com/onqtam/doctest/pull/256) ([zhihaoy](https://github.com/zhihaoy))
- Fix \#253 [\#254](https://github.com/onqtam/doctest/pull/254) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- add alias target for doctest for use in build tree [\#247](https://github.com/onqtam/doctest/pull/247) ([trondhe](https://github.com/trondhe))

## [2.3.3](https://github.com/onqtam/doctest/tree/2.3.3) (2019-06-02)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.3.2...2.3.3)

**Closed issues:**

- Build fails with gcc9 because of -Wstrict-overflow=5 which is too high [\#241](https://github.com/onqtam/doctest/issues/241)
- doctest given defined with short macro name [\#239](https://github.com/onqtam/doctest/issues/239)
- Splitting templated test across different translation units [\#238](https://github.com/onqtam/doctest/issues/238)
- Compile errors with iosfwd.h and Visual Studio 2019 Preview [\#183](https://github.com/onqtam/doctest/issues/183)
- Add CMake test support as catch\_discover\_tests\(\) in Catch2 [\#171](https://github.com/onqtam/doctest/issues/171)

**Merged pull requests:**

- fix \#239 - use long macro name [\#240](https://github.com/onqtam/doctest/pull/240) ([m-bd](https://github.com/m-bd))
- Add doctest\_discover\_tests\(\) [\#236](https://github.com/onqtam/doctest/pull/236) ([RedDwarf69](https://github.com/RedDwarf69))
- Ignore redundant-decls warning on MinGW [\#235](https://github.com/onqtam/doctest/pull/235) ([AMS21](https://github.com/AMS21))
- Fixed meson build file dependency declaration [\#233](https://github.com/onqtam/doctest/pull/233) ([jormundgand](https://github.com/jormundgand))

## [2.3.2](https://github.com/onqtam/doctest/tree/2.3.2) (2019-05-06)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.3.1...2.3.2)

**Closed issues:**

- scripts/bench/run\_all.py : module 'urllib' has no attribute 'urlretrieve' [\#230](https://github.com/onqtam/doctest/issues/230)
- wrong set of tests registered with TEST\_CASE\_TEMPLATE get executed [\#228](https://github.com/onqtam/doctest/issues/228)
- Logging not Working for me [\#227](https://github.com/onqtam/doctest/issues/227)
- Link test runner executable into dll? [\#226](https://github.com/onqtam/doctest/issues/226)
- Linking issue for executables after including doctest in library [\#224](https://github.com/onqtam/doctest/issues/224)
- Strange REQUIRE\_THROWS behaviour [\#223](https://github.com/onqtam/doctest/issues/223)
- Windows clang-cl Wunused-variable warning [\#221](https://github.com/onqtam/doctest/issues/221)
- Update doctest 2.3.1 in bincrafters [\#220](https://github.com/onqtam/doctest/issues/220)
- make install, on 64 bit, installs cmake files into lib instead of lib64 folder  [\#218](https://github.com/onqtam/doctest/issues/218)
- TSAN: data race related to hasLoggedCurrentTestStart [\#217](https://github.com/onqtam/doctest/issues/217)
- REQUIRE\_THROWS\_AS does not support class constructors [\#216](https://github.com/onqtam/doctest/issues/216)
- Build failure on clang 7.0.1 on Fedora 29 [\#214](https://github.com/onqtam/doctest/issues/214)
- add example compatible with -\> https://github.com/report-ci/ [\#212](https://github.com/onqtam/doctest/issues/212)
- No DOCTEST\_WITH\_TESTS? [\#211](https://github.com/onqtam/doctest/issues/211)

**Merged pull requests:**

- Added meson file, to declare a dependency. [\#232](https://github.com/onqtam/doctest/pull/232) ([jormundgand](https://github.com/jormundgand))
- Explicitly specify the doctest\_with\_main C++ standard in CMake. [\#231](https://github.com/onqtam/doctest/pull/231) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Remove architecture check from CMake package [\#225](https://github.com/onqtam/doctest/pull/225) ([mmha](https://github.com/mmha))
- add default install prefix [\#219](https://github.com/onqtam/doctest/pull/219) ([a4z](https://github.com/a4z))
- \[regression\] Workaround MSVC preprocessor issue triggered by REQUIRE\_THROWS [\#213](https://github.com/onqtam/doctest/pull/213) ([zhihaoy](https://github.com/zhihaoy))

## [2.3.1](https://github.com/onqtam/doctest/tree/2.3.1) (2019-03-24)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.3.0...2.3.1)

**Merged pull requests:**

- Add two very simple examples of using doctest with CMake [\#209](https://github.com/onqtam/doctest/pull/209) ([pr0g](https://github.com/pr0g))

## [2.3.0](https://github.com/onqtam/doctest/tree/2.3.0) (2019-03-23)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.2.3...2.3.0)

**Closed issues:**

- Compilation with emscripten fails by default because of signal handling [\#207](https://github.com/onqtam/doctest/issues/207)
- Compilation fails with cl.exe /Zc:wchar\_t- [\#206](https://github.com/onqtam/doctest/issues/206)
- Parallel invocation of doctest's own testsuite via CTest fails [\#202](https://github.com/onqtam/doctest/issues/202)
-  Get the number of passed/failed tests in the code [\#200](https://github.com/onqtam/doctest/issues/200)
- Tests alongside code with multiple executables [\#199](https://github.com/onqtam/doctest/issues/199)
- Cppcheck 1.86 warnings [\#198](https://github.com/onqtam/doctest/issues/198)
- Compiling as Dll maybe is wrong [\#196](https://github.com/onqtam/doctest/issues/196)
- Forward-declaring identifiers in std:: is UB - consider including some of the cheaper C/C++ stdlib headers [\#194](https://github.com/onqtam/doctest/issues/194)
- QtCreator + clang warning about operator \<\< precedence [\#191](https://github.com/onqtam/doctest/issues/191)
- run test fixture from cli [\#190](https://github.com/onqtam/doctest/issues/190)
- Installing doctest using cmake and make fails on Ubuntu 16.04 \(C++11 is not used\) [\#189](https://github.com/onqtam/doctest/issues/189)
- c++17 requirement for testing private members [\#188](https://github.com/onqtam/doctest/issues/188)
- \[feature request\] implement a user-extendable reporter system [\#138](https://github.com/onqtam/doctest/issues/138)
- Same test runs multiple times when written in a header and included with different unnormalized paths [\#45](https://github.com/onqtam/doctest/issues/45)

**Merged pull requests:**

- Fix unmatched bracket in DOCTEST\_TEST\_CASE\_CLASS [\#204](https://github.com/onqtam/doctest/pull/204) ([patstew](https://github.com/patstew))
- Template apply [\#203](https://github.com/onqtam/doctest/pull/203) ([zhihaoy](https://github.com/zhihaoy))
- No undefined behavior per C++ standard in detecting endianness. [\#195](https://github.com/onqtam/doctest/pull/195) ([dimztimz](https://github.com/dimztimz))
- Fix propagating include directories of target doctest\_with\_main [\#193](https://github.com/onqtam/doctest/pull/193) ([dimztimz](https://github.com/dimztimz))
-  Move single header to a separate folder [\#187](https://github.com/onqtam/doctest/pull/187) ([dimztimz](https://github.com/dimztimz))
- Fix Clang format to handle C++11 [\#186](https://github.com/onqtam/doctest/pull/186) ([dimztimz](https://github.com/dimztimz))
- Rename doctest\_impl.h to doctest.cpp for less confusion. [\#185](https://github.com/onqtam/doctest/pull/185) ([dimztimz](https://github.com/dimztimz))

## [2.2.3](https://github.com/onqtam/doctest/tree/2.2.3) (2019-02-10)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.2.2...2.2.3)

**Closed issues:**

- Calling convention needed on a few functions [\#182](https://github.com/onqtam/doctest/issues/182)
- Terminal color is not reset when a test fails with some signal [\#122](https://github.com/onqtam/doctest/issues/122)
- testing private members - ability to write test cases in class bodies [\#76](https://github.com/onqtam/doctest/issues/76)

## [2.2.2](https://github.com/onqtam/doctest/tree/2.2.2) (2019-01-28)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.2.1...2.2.2)

**Closed issues:**

- Add way to override getCurrentTicks\(\) implementation [\#178](https://github.com/onqtam/doctest/issues/178)
- Wrap \<csignal\> include with ifdef [\#177](https://github.com/onqtam/doctest/issues/177)
- How to stop doctest hijack unhandled exceptions? [\#176](https://github.com/onqtam/doctest/issues/176)
- Change the include path of the `doctest` CMake interface target so users need to specify the folder as well [\#175](https://github.com/onqtam/doctest/issues/175)
- Reduce scope of DebugOutputWindowReporter instance [\#174](https://github.com/onqtam/doctest/issues/174)
- Can logging \(INFO\) be used in helper class outside of TEST\_CASE? [\#169](https://github.com/onqtam/doctest/issues/169)

**Merged pull requests:**

- Change the include path in examples as \#175 [\#180](https://github.com/onqtam/doctest/pull/180) ([ncihnegn](https://github.com/ncihnegn))
- Fix CMake include path \#175 [\#179](https://github.com/onqtam/doctest/pull/179) ([ncihnegn](https://github.com/ncihnegn))

## [2.2.1](https://github.com/onqtam/doctest/tree/2.2.1) (2019-01-15)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.2.0...2.2.1)

**Closed issues:**

- the `--no-throw` option shouldn't affect `\<LEVEL\>\_NOTHROW` asserts [\#173](https://github.com/onqtam/doctest/issues/173)
- Make doctest work with XCode 6 and 7 \(no support for C++11 thread\_local\) [\#172](https://github.com/onqtam/doctest/issues/172)
- Print vector content. [\#170](https://github.com/onqtam/doctest/issues/170)
- Conan package [\#103](https://github.com/onqtam/doctest/issues/103)
- \[feature request\] Thread-safety for asserts and logging facilities [\#4](https://github.com/onqtam/doctest/issues/4)

## [2.2.0](https://github.com/onqtam/doctest/tree/2.2.0) (2018-12-05)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.1.0...2.2.0)

**Closed issues:**

- remove the FAST\_ versions of the binary asserts \(not a breaking change!\) [\#167](https://github.com/onqtam/doctest/issues/167)
- \[compile times\] make the DOCTEST\_CONFIG\_SUPER\_FAST\_ASSERTS identifier affect normal asserts too [\#166](https://github.com/onqtam/doctest/issues/166)

## [2.1.0](https://github.com/onqtam/doctest/tree/2.1.0) (2018-11-30)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.0.1...2.1.0)

**Closed issues:**

- doctest::String ctor with non-zero terminated string [\#165](https://github.com/onqtam/doctest/issues/165)
- thread\_local is not supported on iOS 9.0 [\#164](https://github.com/onqtam/doctest/issues/164)
- Compiler error on Android NDK r18 [\#163](https://github.com/onqtam/doctest/issues/163)
- \[question\] One setup for multiple tests [\#160](https://github.com/onqtam/doctest/issues/160)
- clang unwanted warning in user code [\#156](https://github.com/onqtam/doctest/issues/156)
- Unsigned integer overflow in fileOrderComparator [\#151](https://github.com/onqtam/doctest/issues/151)
- ThreadSanitizer: signal-unsafe call inside of a signal [\#147](https://github.com/onqtam/doctest/issues/147)
- Feature request: check for exception string \(like Catch's CHECK\_THROWS\_WITH\) [\#97](https://github.com/onqtam/doctest/issues/97)

**Merged pull requests:**

- Fixed build error under Android NDK [\#162](https://github.com/onqtam/doctest/pull/162) ([tals](https://github.com/tals))
- Added clang-7 to travis build [\#161](https://github.com/onqtam/doctest/pull/161) ([AMS21](https://github.com/AMS21))
- Remove clang-tidy warnings for static fields created by doctest [\#159](https://github.com/onqtam/doctest/pull/159) ([rantasub](https://github.com/rantasub))
- Make it possible to change the command line options prefix [\#158](https://github.com/onqtam/doctest/pull/158) ([tbleher](https://github.com/tbleher))

## [2.0.1](https://github.com/onqtam/doctest/tree/2.0.1) (2018-10-24)
[Full Changelog](https://github.com/onqtam/doctest/compare/2.0.0...2.0.1)

**Closed issues:**

- macro name collision with google log [\#157](https://github.com/onqtam/doctest/issues/157)
- Add \#define to not run tests by default [\#152](https://github.com/onqtam/doctest/issues/152)
- REQUIRE\_THROWS\_MESSAGE not checking message correctly [\#150](https://github.com/onqtam/doctest/issues/150)
- Test case passes even though subcase failed [\#149](https://github.com/onqtam/doctest/issues/149)

**Merged pull requests:**

- Correctly document when a main\(\) entry point will be created [\#155](https://github.com/onqtam/doctest/pull/155) ([tbleher](https://github.com/tbleher))
- Correct format string for unsigned char [\#154](https://github.com/onqtam/doctest/pull/154) ([tbleher](https://github.com/tbleher))

## [2.0.0](https://github.com/onqtam/doctest/tree/2.0.0) (2018-08-23)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.9...2.0.0)

**Closed issues:**

- MSVC 2017 15.8.1, New Warnings as Errors [\#144](https://github.com/onqtam/doctest/issues/144)
- Windows clang-cl Wdeprecated-declarations warnings [\#143](https://github.com/onqtam/doctest/issues/143)
- Logo Proposal for Doctest [\#141](https://github.com/onqtam/doctest/issues/141)
- PCH Support [\#140](https://github.com/onqtam/doctest/issues/140)
- improve compile times even further [\#139](https://github.com/onqtam/doctest/issues/139)
- !!! BREAKING CHANGE !!! - Move to C++11 for next version of the library [\#137](https://github.com/onqtam/doctest/issues/137)
- getCurrentTicks producing warning on MinGW [\#133](https://github.com/onqtam/doctest/issues/133)
- \[enhancement\] Add support for "stand-alone assertions". [\#114](https://github.com/onqtam/doctest/issues/114)

**Merged pull requests:**

- Suppress compiler warning on MinGW [\#134](https://github.com/onqtam/doctest/pull/134) ([AMS21](https://github.com/AMS21))

## [1.2.9](https://github.com/onqtam/doctest/tree/1.2.9) (2018-05-10)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.8...1.2.9)

**Closed issues:**

- GCC 8.0 std::uncaught\_exception\(\) is deprecated  [\#130](https://github.com/onqtam/doctest/issues/130)
- Signal stack size too small on Linux [\#129](https://github.com/onqtam/doctest/issues/129)
- Support Intel Compiler [\#128](https://github.com/onqtam/doctest/issues/128)
- Please add support for MSVC 2005 [\#127](https://github.com/onqtam/doctest/issues/127)
- scan-build report "Dereference of null pointer" for function wildcmp [\#124](https://github.com/onqtam/doctest/issues/124)
- !!! BREAKING CHANGE \(console output only\)  !!! - Emulate the error/warning format emitted by native compiler gcc/clang/msvc when printing test failures in the log [\#123](https://github.com/onqtam/doctest/issues/123)
- ARM builds: FTBFS on armhf - error: cast from 'const char\*' to 'const  [\#118](https://github.com/onqtam/doctest/issues/118)

**Merged pull requests:**

- Exclude Intel from GCC compiler check [\#132](https://github.com/onqtam/doctest/pull/132) ([smcallis](https://github.com/smcallis))
- Fix deprecated-declarations warning with GCC-8.0 [\#131](https://github.com/onqtam/doctest/pull/131) ([AMS21](https://github.com/AMS21))

## [1.2.8](https://github.com/onqtam/doctest/tree/1.2.8) (2018-03-10)
[Full Changelog](https://github.com/onqtam/doctest/compare/1.2.7...1.2.8)

**Closed issues:**

- ARM64 builds: templated\_test\_cases.cpp test fails [\#119](https://github.com/onqtam/doctest/issues/119)

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
- Warnings raised to error with latest MSVC version [\#107](https://github.com/onqtam/doctest/issues/107)
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

- Added an option to not install Doctest in the CMake scripts [\#96](https://github.com/onqtam/doctest/pull/96) ([nm17](https://github.com/nm17))
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
- Add option to query if code is being ran inside of a test - doctest::is\_running\_in\_test [\#56](https://github.com/onqtam/doctest/issues/56)
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

- don't show green when tests fail [\#26](https://github.com/onqtam/doctest/pull/26) ([ferkulat](https://github.com/ferkulat))
- Include "program code" in example [\#15](https://github.com/onqtam/doctest/pull/15) ([martinmoene](https://github.com/martinmoene))

## [1.0.0](https://github.com/onqtam/doctest/tree/1.0.0) (2016-05-22)
**Merged pull requests:**

- Reduce the header size for test users [\#3](https://github.com/onqtam/doctest/pull/3) ([zah](https://github.com/zah))
- Add a Gitter chat badge to README.md [\#1](https://github.com/onqtam/doctest/pull/1) ([gitter-badger](https://github.com/gitter-badger))



\* *This Change Log was automatically generated by [github_changelog_generator](https://github.com/skywinder/Github-Changelog-Generator)*