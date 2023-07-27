# Change Log

## [v2.4.11](https://github.com/doctest/doctest/tree/v2.4.11) (2023-03-15)

[Full Changelog](https://github.com/doctest/doctest/compare/v2.4.10...v2.4.11)

## [v2.4.10](https://github.com/doctest/doctest/tree/v2.4.10) (2023-02-27)

[Full Changelog](https://github.com/doctest/doctest/compare/v2.4.9...v2.4.10)

## [v2.4.9](https://github.com/doctest/doctest/tree/v2.4.9) (2022-06-18)
[Full Changelog](https://github.com/doctest/doctest/compare/v2.4.8...v2.4.9)

**Closed issues:**
- Visual Studio's Test Explorer and Resharper C\+\+'s Unit Test Explorer don't see Doctest's tests \#661 ([KulaGGin](https://github.com/KulaGGin))
- How to get detailed information about testcases failing due to thrown exceptions? \#660 ([NiklasKappel](https://github.com/NiklasKappel))
- Add clang\-tidy integration and fix all warnings \#659 ([Saalvage](https://github.com/Saalvage))
- Avoid static init problem in insufficient\_procs\(\) \(MPI\) \#657 ([starintheuniverse](https://github.com/starintheuniverse))
- Use MPI\_Isend in MpiConsoleReporter to avoid deadlock \#656 ([starintheuniverse](https://github.com/starintheuniverse))
- Deadlock in MpiConsoleReporter when root rank fails assert \#655 ([starintheuniverse](https://github.com/starintheuniverse))
- Cleanup of DOCTEST\_DO\_BINARY\_EXPRESSION\_COMPARISON\. Fixes \#651 \#652 ([iboB](https://github.com/iboB))
- Comparison with implicit cast from non\-const value can't be decomposed \#651 ([iboB](https://github.com/iboB))
- Local structured bindings cannot be used in CHECK macros \(since 2\.4\.8\) \#647 ([pragmaxwell](https://github.com/pragmaxwell))
- Add tests for DOCTEST\_CONFIG\_USE\_STD\_HEADERS \#643 ([Saalvage](https://github.com/Saalvage))
- Stringification amendments \#642 ([Saalvage](https://github.com/Saalvage))
- Clean up defines a bit; Implement \#439 \#641 ([Saalvage](https://github.com/Saalvage))
- Fix \#508 \#640 ([Saalvage](https://github.com/Saalvage))
- Fix \#508 \#639 ([Saalvage](https://github.com/Saalvage))
- New doctest version gives me an error: reference to local binding '\.\.\.' declared in enclosing function 'DOCTEST\_ANON\_FUNC\_16' \#638 ([a4z](https://github.com/a4z))
- The tutorial example does not work \(linker errors\) with clang 10 \#637 ([sixcy](https://github.com/sixcy))
- Implementing \`DOCTEST\_ASSERT\_IMPLEMENT\_1\` as lambda prevents testing structured bindings \#636 ([ChrisThrasher](https://github.com/ChrisThrasher))
- re\-re\-remove overly restrictive minimum version of meson \#635 ([eli-schwartz](https://github.com/eli-schwartz))
- Fix move\-only types failing to decompose correctly \#634 ([Saalvage](https://github.com/Saalvage))
- Weird compilation error when using CHECK\_THROWS/CHECK\_THROWS\_AS on Visual Studio 2019 with no exceptions \#633 ([yeputons](https://github.com/yeputons))
- Error triggered by comparing typeid with new doctest 2\.4\.8 \#632 ([JazzSuite](https://github.com/JazzSuite))
- Improve Mac PowerPC support \#631 ([ryandesign](https://github.com/ryandesign))
- issue introduced in 2\.4\.7 \#630 ([onqtam](https://github.com/onqtam))
- Decompose expressions containing the spaceship operator \#629 ([falbrechtskirchinger](https://github.com/falbrechtskirchinger))
- added nolint for cert\-err58 \#628 ([serguei-k](https://github.com/serguei-k))
- Fix properties not being passed in doctest\_discover\_tests \#626 ([quantumsteve](https://github.com/quantumsteve))
- Config no multithreading \#625 ([Saalvage](https://github.com/Saalvage))
- wasm\*\-support? \#624 ([FrozenSource](https://github.com/FrozenSource))
- Fix MPI extension to work with no parallel tests \#623 ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- string comparison leads to gotting stuck \#622 ([laoshaw](https://github.com/laoshaw))
- doctest\_discover\_tests no longer sets ENVIRONMENT variables for discovered tests\.  \#621 ([quantumsteve](https://github.com/quantumsteve))
- Add contains option to checks\. \#620 ([MFraters](https://github.com/MFraters))
- Feature request: CHECK\_THROWS\_WITH with contains option \#619 ([MFraters](https://github.com/MFraters))
- Add alias target for doctest\_with\_main \#617 ([jessestricker](https://github.com/jessestricker))
- Allow escaping backslash with backslash in filters \(\#614\) \#616 ([yeputons](https://github.com/yeputons))
- Fix operator<< \#615 ([Saalvage](https://github.com/Saalvage))
- Correct minor typos \#613 ([utilForever](https://github.com/utilForever))
- Fix MPI extension to work if launched without mpirun/mpiexec \#612 ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- Fix mpi subcase \#611 ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- compilation error with custom operator== defined in namespace \#610 ([zvyagin1](https://github.com/zvyagin1))
- Regression: Clang\-Tidy warnings in 2\.4\.8 \#607 ([nlohmann](https://github.com/nlohmann))
- Internal compiler error with GCC 7\.5 \#606 ([foonathan](https://github.com/foonathan))
- tagging convension has changed? \#605 ([kloczek](https://github.com/kloczek))
- Update Doctest in vcpkg to 2\.4\.8 \#604 ([gc435](https://github.com/gc435))
- Add IsNaN operator\! \#603 ([Saalvage](https://github.com/Saalvage))
- Ignored generated files from CMake, OSX, Xcode, and VS \#602 ([LeonBrands](https://github.com/LeonBrands))
- Move roadmap and wipe it clean \#601 ([Saalvage](https://github.com/Saalvage))
- removes a duplicate word 'most' in configuration\.md \#599 ([krishnakumarg1984](https://github.com/krishnakumarg1984))
- Fix subcase reentry \#598 ([Saalvage](https://github.com/Saalvage))
- Loop\-generated \`SUBCASE\`s are not run \#597 ([yeputons](https://github.com/yeputons))
- Void \#596 ([Saalvage](https://github.com/Saalvage))
- Add flag that forces custom stringification methods to be provided \#595 ([Saalvage](https://github.com/Saalvage))
- Fix coverage \#594 ([Saalvage](https://github.com/Saalvage))
- TEST CODECOV PR BEHAVIOR \#593 ([Saalvage](https://github.com/Saalvage))
- Ignore CMake and MacOS generated files \#592 ([LeonBrands](https://github.com/LeonBrands))
- Feature request: option to disable fallback "\{?\}" stringifier \#591 ([YarikTH](https://github.com/YarikTH))
- Add tests for default stringification result of doctest \#590 ([YarikTH](https://github.com/YarikTH))
- Feature config ret vals \#589 ([Saalvage](https://github.com/Saalvage))
- DOCTEST\_CONFIG\_ASSERT\_RETURN\_VALUES \#588 ([Saalvage](https://github.com/Saalvage))
- Support pretty printing of container based on heuristics \#587 ([YarikTH](https://github.com/YarikTH))
- Refactor stringification \#585 ([Saalvage](https://github.com/Saalvage))
- Feature: Better NaN \#584 ([Saalvage](https://github.com/Saalvage))
- Nan check \#582 ([Saalvage](https://github.com/Saalvage))
- Update roadmap following maintainer change \#581 ([eyalroz](https://github.com/eyalroz))
- Regression between 2\.4\.6 and 2\.4\.7 \#571 ([YarikTH](https://github.com/YarikTH))
- build failure with gcc\-11\.2 when using user declared operator<<\(ostream, vector\) \#551 ([nlitsme](https://github.com/nlitsme))
- variable maximum is assigned 6206517616395625 instead of the actual return value which is 5\.0 \#530 ([kk723](https://github.com/kk723))
- toString can call existing user\-defined toString through ADL incorrectly \#508 ([zeux](https://github.com/zeux))
- \[Coverity\] Concurrent data access violations \(MISSING\_LOCK\) doctest\.h: 5838 in doctest::<unnamed>::ConsoleReporter::test\_case\_start\(const doctest::TestCaseData &\)\(\) \#486 ([jiridanek](https://github.com/jiridanek))
- Provide an error message if REQUIRE \(or other disabled assertion macros\) are used when exceptions are disabled \#439 ([alexeyr](https://github.com/alexeyr))
- Conflict with templated toString function \#420 ([TillHeinzel](https://github.com/TillHeinzel))
- \-tc does not work with comma in names \#398 ([martinus](https://github.com/martinus))
- Compile error on MSVC2019 with any macro which involves stringification of std::string \(asserts, INFO, etc\.\) when <ostream> isn't included \#381 ([nyanpasu64](https://github.com/nyanpasu64))
- the dll example doesn't run correctly on Windows with MinGW \#375 ([GregKon](https://github.com/GregKon))
- add basic conan recipe \#354 ([trondhe](https://github.com/trondhe))
- CHECK\_MESSAGE\(\) should accept temporaries \#337 ([eyalroz](https://github.com/eyalroz))
- stringify of cstring literals doesn't work out of the box with separate test\_driver\.cpp \#329 ([teichert](https://github.com/teichert))
- warning : function declared 'noreturn' should not return \[\-Winvalid\-noreturn\] \#307 ([Vexthil](https://github.com/Vexthil))
- Test cases containing a comma cannot be run individually \#297 ([Tradias](https://github.com/Tradias))
- \[bug\] Can't compile the tests because of mutex, that is declared in the doctest \#242 ([BrunaoW](https://github.com/BrunaoW))
- The \`CHECK\` macro conflicts with Boost\.Beast \(and surely others\) \#234 ([reddwarf69](https://github.com/reddwarf69))
- Feature request: check if a \`float\` or \`double\` is NaN \#105 ([iamthad](https://github.com/iamthad))

**Merged pull requests:**
- Add clang\-tidy integration and fix all warnings \#659 ([Saalvage](https://github.com/Saalvage))
- Avoid static init problem in insufficient\_procs\(\) \(MPI\) \#657 ([starintheuniverse](https://github.com/starintheuniverse))
- Use MPI\_Isend in MpiConsoleReporter to avoid deadlock \#656 ([starintheuniverse](https://github.com/starintheuniverse))
- Cleanup of DOCTEST\_DO\_BINARY\_EXPRESSION\_COMPARISON\. Fixes \#651 \#652 ([iboB](https://github.com/iboB))
- Add tests for DOCTEST\_CONFIG\_USE\_STD\_HEADERS \#643 ([Saalvage](https://github.com/Saalvage))
- Stringification amendments \#642 ([Saalvage](https://github.com/Saalvage))
- Clean up defines a bit; Implement \#439 \#641 ([Saalvage](https://github.com/Saalvage))
- Fix \#508 \#640 ([Saalvage](https://github.com/Saalvage))
- re\-re\-remove overly restrictive minimum version of meson \#635 ([eli-schwartz](https://github.com/eli-schwartz))
- Fix move\-only types failing to decompose correctly \#634 ([Saalvage](https://github.com/Saalvage))
- Improve Mac PowerPC support \#631 ([ryandesign](https://github.com/ryandesign))
- added nolint for cert\-err58 \#628 ([serguei-k](https://github.com/serguei-k))
- Fix properties not being passed in doctest\_discover\_tests \#626 ([quantumsteve](https://github.com/quantumsteve))
- Config no multithreading \#625 ([Saalvage](https://github.com/Saalvage))
- Fix MPI extension to work with no parallel tests \#623 ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- Add contains option to checks\. \#620 ([MFraters](https://github.com/MFraters))
- Add alias target for doctest\_with\_main \#617 ([jessestricker](https://github.com/jessestricker))
- Allow escaping backslash with backslash in filters \(\#614\) \#616 ([yeputons](https://github.com/yeputons))
- Fix operator<< \#615 ([Saalvage](https://github.com/Saalvage))
- Fix MPI extension to work if launched without mpirun/mpiexec \#612 ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- Fix mpi subcase \#611 ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- Add IsNaN operator\! \#603 ([Saalvage](https://github.com/Saalvage))
- Move roadmap and wipe it clean \#601 ([Saalvage](https://github.com/Saalvage))
- removes a duplicate word 'most' in configuration\.md \#599 ([krishnakumarg1984](https://github.com/krishnakumarg1984))
- Fix subcase reentry \#598 ([Saalvage](https://github.com/Saalvage))
- Add flag that forces custom stringification methods to be provided \#595 ([Saalvage](https://github.com/Saalvage))
- Fix coverage \#594 ([Saalvage](https://github.com/Saalvage))
- Ignore CMake and MacOS generated files \#592 ([LeonBrands](https://github.com/LeonBrands))
- Feature config ret vals \#589 ([Saalvage](https://github.com/Saalvage))
- Refactor stringification \#585 ([Saalvage](https://github.com/Saalvage))
- Feature: Better NaN \#584 ([Saalvage](https://github.com/Saalvage))
- Nan check \#582 ([Saalvage](https://github.com/Saalvage))

## [v2.4.8](https://github.com/doctest/doctest/tree/v2.4.8) (2022-01-10)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.7...v2.4.8)

**Closed issues:**

- \[meta\] Change git tagging pattern [\#579](https://github.com/doctest/doctest/issues/579)
- TEST\_CASE\_TEMPLATE causes "-Wunused-local-typedef" warning on Clang [\#577](https://github.com/doctest/doctest/issues/577)
- Regression between 2.4.6 and 2.4.7 with Visual Studio 2015 [\#573](https://github.com/doctest/doctest/issues/573)
- Regression between 2.4.6 and 2.4.7 [\#571](https://github.com/doctest/doctest/issues/571)
- Compilation error on MSVS2019 with ClangCL [\#570](https://github.com/doctest/doctest/issues/570)
- Compilation errors on MSVC 2015 after doctest update to 2.4.7 [\#568](https://github.com/doctest/doctest/issues/568)
- `g\_oss` is causing incorrect stringification results [\#567](https://github.com/doctest/doctest/issues/567)
- MSVC warnings leak through when using the library as a single header with /Wall [\#565](https://github.com/doctest/doctest/issues/565)
- \[PROJECT ANNOUNCEMENT\] Looking for maintainers for Doctest [\#554](https://github.com/doctest/doctest/issues/554)
- Is this still maintained? [\#537](https://github.com/doctest/doctest/issues/537)
- \[Feature request\] CHECK could return the value of expression [\#496](https://github.com/doctest/doctest/issues/496)
- Feature: check or return false [\#426](https://github.com/doctest/doctest/issues/426)
- Undefined reference of `operator\<\<\(ostream&, const string&\)` when compiling with clang 10 and libc++ 10 on Ubuntu 16.04.6 LTS [\#356](https://github.com/doctest/doctest/issues/356)
- Doctest is not able to compile on OSX [\#126](https://github.com/doctest/doctest/issues/126)

**Merged pull requests:**

- Continuous Integration Refactor [\#580](https://github.com/doctest/doctest/pull/580) ([Saalvage](https://github.com/Saalvage))
- Fix semicolon enforcement [\#578](https://github.com/doctest/doctest/pull/578) ([Saalvage](https://github.com/Saalvage))
- Fix unused variable 2 [\#576](https://github.com/doctest/doctest/pull/576) ([Saalvage](https://github.com/Saalvage))
- Alternative approach to Windows color initialization [\#575](https://github.com/doctest/doctest/pull/575) ([Saalvage](https://github.com/Saalvage))
- Assertions returning booleans [\#574](https://github.com/doctest/doctest/pull/574) ([Saalvage](https://github.com/Saalvage))
- Fix the thread-local string-stream [\#569](https://github.com/doctest/doctest/pull/569) ([Saalvage](https://github.com/Saalvage))
- Clean up warning suppression a bit; Fixes \#565 [\#566](https://github.com/doctest/doctest/pull/566) ([Saalvage](https://github.com/Saalvage))
- Add Universal Windows Platform support [\#558](https://github.com/doctest/doctest/pull/558) ([isaevil](https://github.com/isaevil))

## [2.4.7](https://github.com/doctest/doctest/tree/2.4.7) (2021-12-10)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.6...2.4.7)

**Implemented enhancements:**

- Add a default Bazel BUILD file [\#433](https://github.com/doctest/doctest/issues/433)

**Fixed bugs:**

- Stack-buffer-overflow probably because char array is viewed as NULL terminated string [\#476](https://github.com/doctest/doctest/issues/476)

**Closed issues:**

- "C4834: discarding return value" with REQUIRE\_THROWS [\#549](https://github.com/doctest/doctest/issues/549)
- Xcode 11.3 is gone from macOS-latest \(=macOS-11\) [\#547](https://github.com/doctest/doctest/issues/547)
- is it possible to define dependency for CHECKs [\#545](https://github.com/doctest/doctest/issues/545)
- Output summary explanation [\#541](https://github.com/doctest/doctest/issues/541)
- compiler errors in doctest.h using cmake in CLion [\#540](https://github.com/doctest/doctest/issues/540)
- Fails to build in VS2013 because of constexpr [\#539](https://github.com/doctest/doctest/issues/539)
- -Wreserved-identifier warnings with Clang 13.0.0 [\#536](https://github.com/doctest/doctest/issues/536)
- Build fails with latest MSVC 2019 \(v16.11\) due to /WX [\#535](https://github.com/doctest/doctest/issues/535)
- VS 16.11 warning about unreferenced function with internal linkage [\#533](https://github.com/doctest/doctest/issues/533)
- Faq googletest mocking dead link [\#532](https://github.com/doctest/doctest/issues/532)
- FR: Documentation: FAQ: Add sectoin 'multiple files' [\#526](https://github.com/doctest/doctest/issues/526)
- CMAKE: doctest\_discover\_tests\(\) error when ADD\_LABELS is not specified [\#524](https://github.com/doctest/doctest/issues/524)
- Register tests based on test data available [\#521](https://github.com/doctest/doctest/issues/521)
- naming override in different testcase files [\#517](https://github.com/doctest/doctest/issues/517)
- Segmentation fault during the compilation without the copy elision optimization [\#515](https://github.com/doctest/doctest/issues/515)
- Compiler warnings on Xcode 12.5 [\#514](https://github.com/doctest/doctest/issues/514)
- Using filter `-sc` does not work properly? [\#513](https://github.com/doctest/doctest/issues/513)
- \[question\] Example of tests in production code & DLLs & shared libraries? [\#511](https://github.com/doctest/doctest/issues/511)
- Dumping fixture state to disk on error [\#509](https://github.com/doctest/doctest/issues/509)
- Macros construct reserved identifiers [\#507](https://github.com/doctest/doctest/issues/507)
- Running doctest on embedded ARM Cortex µCs [\#506](https://github.com/doctest/doctest/issues/506)
- Asserts Outside of Tests Example Does Not Link [\#504](https://github.com/doctest/doctest/issues/504)
- \[FEATURE REQUEST\] Quiet flag [\#503](https://github.com/doctest/doctest/issues/503)
- Compile error on Intel C++ Classic Compilers [\#502](https://github.com/doctest/doctest/issues/502)
- compiling doctest in 32-bit with \_\_stdcall calling convention fails [\#500](https://github.com/doctest/doctest/issues/500)
- Duplicate 'const' compilation error from TEST\_CASE\_CLASS macro [\#498](https://github.com/doctest/doctest/issues/498)
- Packed fields can't be accessed in 2.4.6 [\#495](https://github.com/doctest/doctest/issues/495)
- Dangling pointers with .str\(\).c\_str\(\) [\#494](https://github.com/doctest/doctest/issues/494)
- Automatic adding of TEST\_SUITE labels to discovered tests fails if ADD\_LABELS not set [\#489](https://github.com/doctest/doctest/issues/489)
- Adding a bunch of REQUIRE/CHECK utilities [\#487](https://github.com/doctest/doctest/issues/487)
- Warning C4114 in MSVC [\#485](https://github.com/doctest/doctest/issues/485)
- Own repository [\#410](https://github.com/doctest/doctest/issues/410)
- Linking problem with Clang 10 on Windows [\#362](https://github.com/doctest/doctest/issues/362)
- Add option not to print the intro text [\#342](https://github.com/doctest/doctest/issues/342)
- \[Feature\] Better integration with tools \(VS Code Test Adapter Extension\) [\#320](https://github.com/doctest/doctest/issues/320)
- vscode test explorer [\#303](https://github.com/doctest/doctest/issues/303)
- Want an option not to print any intro [\#245](https://github.com/doctest/doctest/issues/245)
- Add way to disable printing of intro [\#181](https://github.com/doctest/doctest/issues/181)

**Merged pull requests:**

- Make String::operator+ non-member [\#564](https://github.com/doctest/doctest/pull/564) ([Saalvage](https://github.com/Saalvage))
- Add -minimal flag [\#562](https://github.com/doctest/doctest/pull/562) ([Saalvage](https://github.com/Saalvage))
- Quiet flag [\#561](https://github.com/doctest/doctest/pull/561) ([Saalvage](https://github.com/Saalvage))
- Fix redefinition error while using double time DOCTEST\_ANONYMOUS\(DOCTEST\_CAPTURE\_\) [\#557](https://github.com/doctest/doctest/pull/557) ([isaevil](https://github.com/isaevil))
- Fix error: missing initializer for member doctest::detail::TestSuite [\#556](https://github.com/doctest/doctest/pull/556) ([isaevil](https://github.com/isaevil))
- Xcode 11.3 with macos 10.15 [\#548](https://github.com/doctest/doctest/pull/548) ([jsoref](https://github.com/jsoref))
- Spelling [\#546](https://github.com/doctest/doctest/pull/546) ([jsoref](https://github.com/jsoref))
- Fix build with -Wunused-but-set-variable [\#543](https://github.com/doctest/doctest/pull/543) ([jktjkt](https://github.com/jktjkt))
- build\(meson\): use `override\_dependency` if supported [\#538](https://github.com/doctest/doctest/pull/538) ([Tachi107](https://github.com/Tachi107))
- Fix google death test URL [\#528](https://github.com/doctest/doctest/pull/528) ([emrecil](https://github.com/emrecil))
- Fixing issue with doctestAddTests.cmake [\#527](https://github.com/doctest/doctest/pull/527) ([jharmer95](https://github.com/jharmer95))
- Replace gendered pronouns [\#525](https://github.com/doctest/doctest/pull/525) ([mletterle](https://github.com/mletterle))
- Fixed intel compiler parser bug. Should fix \#502 [\#523](https://github.com/doctest/doctest/pull/523) ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- specifying working directory for execute\_process in doctest\_discover\_tests [\#518](https://github.com/doctest/doctest/pull/518) ([philbucher](https://github.com/philbucher))
- Fix the logic that depends on optional copy elision optimization [\#516](https://github.com/doctest/doctest/pull/516) ([ivankochin](https://github.com/ivankochin))
- Fix reserved identifiers [\#510](https://github.com/doctest/doctest/pull/510) ([ts826848](https://github.com/ts826848))
- Fix build with GCC 11 [\#505](https://github.com/doctest/doctest/pull/505) ([jktjkt](https://github.com/jktjkt))
- minor fixes in MPI docs [\#499](https://github.com/doctest/doctest/pull/499) ([philbucher](https://github.com/philbucher))
- Add a minimal bazel config [\#497](https://github.com/doctest/doctest/pull/497) ([elliottt](https://github.com/elliottt))
- Handle escaped commas in parsed arguments [\#493](https://github.com/doctest/doctest/pull/493) ([friendlyanon](https://github.com/friendlyanon))
- Fixes Issue 476 . When running executables with "-s" stringifyBinaryE… [\#491](https://github.com/doctest/doctest/pull/491) ([navinp0304](https://github.com/navinp0304))
- Set variable to 0 if not set [\#490](https://github.com/doctest/doctest/pull/490) ([shivupa](https://github.com/shivupa))

## [2.4.6](https://github.com/doctest/doctest/tree/2.4.6) (2021-03-22)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.5...2.4.6)

**Fixed bugs:**

- REQUIRE does not compile when operator== in different namespace [\#443](https://github.com/doctest/doctest/issues/443)
- Using templated operator== inside TEST\_CASE changes deduced types of forwarding references [\#399](https://github.com/doctest/doctest/issues/399)

**Closed issues:**

- CMake doesn't link package [\#483](https://github.com/doctest/doctest/issues/483)
- Assertions are slow when running on Windows with a debugger attached [\#481](https://github.com/doctest/doctest/issues/481)
- Get list of registered test-case names [\#479](https://github.com/doctest/doctest/issues/479)
- Can't compile with glibc master \(future 2.34\): SIGSTKSZ is no longer a constant [\#473](https://github.com/doctest/doctest/issues/473)
- How to use Doctest with Github Actions [\#472](https://github.com/doctest/doctest/issues/472)
- Link error \(multiple definition...\) in simple project [\#470](https://github.com/doctest/doctest/issues/470)
- INFO does not compile when used like a function call [\#469](https://github.com/doctest/doctest/issues/469)
- std::uncaught\_exceptions is only available if compiling for macOS 10.12 or above [\#466](https://github.com/doctest/doctest/issues/466)
- Compile failure with WinRT on 2.4.5 [\#465](https://github.com/doctest/doctest/issues/465)

**Merged pull requests:**

- Improve speed with attached debugger \(Windows\) [\#482](https://github.com/doctest/doctest/pull/482) ([pgroke](https://github.com/pgroke))
- Convert to bool by casting, rather than double negation [\#480](https://github.com/doctest/doctest/pull/480) ([kitegi](https://github.com/kitegi))
- Fix compile error when targeting macOS version earlier and macOS 10.12 [\#478](https://github.com/doctest/doctest/pull/478) ([SamWindell](https://github.com/SamWindell))
- Fix MSVC linter warning about uninitialized TestSuite variables [\#471](https://github.com/doctest/doctest/pull/471) ([Reedbeta](https://github.com/Reedbeta))
- REQUIRE does not compile when operator== in different namespace \#443 . [\#468](https://github.com/doctest/doctest/pull/468) ([navinp0304](https://github.com/navinp0304))
- Automatically add TEST\_SUITE labels to discovered tests [\#464](https://github.com/doctest/doctest/pull/464) ([shivupa](https://github.com/shivupa))

## [2.4.5](https://github.com/doctest/doctest/tree/2.4.5) (2021-02-02)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.4...2.4.5)

**Closed issues:**

- Stack buffer overflow in `String` constructor [\#460](https://github.com/doctest/doctest/issues/460)
- Suppress warnings from clang-tidy [\#459](https://github.com/doctest/doctest/issues/459)
- compilation issue in MSVC when defining DOCTEST\_THREAD\_LOCAL to static [\#458](https://github.com/doctest/doctest/issues/458)
- nvcc compiler warning; doctest.h\(4138\): warning : expression has no effect [\#454](https://github.com/doctest/doctest/issues/454)
- Use of std::atomic can slow down multithreaded tests [\#452](https://github.com/doctest/doctest/issues/452)

**Merged pull requests:**

- Fix compilation on case-sensitive filesystems [\#463](https://github.com/doctest/doctest/pull/463) ([jhasse](https://github.com/jhasse))
- Use function-like macros for prefixless macro names [\#462](https://github.com/doctest/doctest/pull/462) ([tbleher](https://github.com/tbleher))
- Implement a multi lane atomic for assertion counts [\#453](https://github.com/doctest/doctest/pull/453) ([martinus](https://github.com/martinus))

## [2.4.4](https://github.com/doctest/doctest/tree/2.4.4) (2020-12-25)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.3...2.4.4)

**Closed issues:**

- 2.4.2: build fails [\#450](https://github.com/doctest/doctest/issues/450)
- combine the same tests for different build configurations from multiple shared objects without having symbol clashes [\#436](https://github.com/doctest/doctest/issues/436)
- Issue with GitHub Security Scanning: gmtime [\#423](https://github.com/doctest/doctest/issues/423)

## [2.4.3](https://github.com/doctest/doctest/tree/2.4.3) (2020-12-16)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.2...2.4.3)

## [2.4.2](https://github.com/doctest/doctest/tree/2.4.2) (2020-12-15)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.1...2.4.2)

**Closed issues:**

- DOCTEST\_CHECK\_THROWS\_WITH\_AS fails to work with dependant exception type [\#447](https://github.com/doctest/doctest/issues/447)
- MSVC warnings: narrowing conversion, signed/unsigned mismatch [\#446](https://github.com/doctest/doctest/issues/446)
-  log contexts for failures in JUnit reporter [\#441](https://github.com/doctest/doctest/issues/441)
- MinGW "'mutex' in namespace 'std' does not name a type" error. [\#438](https://github.com/doctest/doctest/issues/438)
- Test runner thread initialization [\#435](https://github.com/doctest/doctest/issues/435)
- PLATFORM is misdetected on MacOSX Big Sur [\#415](https://github.com/doctest/doctest/issues/415)
- CHECK\_EQ with enum values [\#276](https://github.com/doctest/doctest/issues/276)

**Merged pull requests:**

- Squash MSVC warnings when including ntstatus.h [\#449](https://github.com/doctest/doctest/pull/449) ([nickhutchinson](https://github.com/nickhutchinson))
- Add MAIN\_PROJECT check for test option [\#445](https://github.com/doctest/doctest/pull/445) ([globberwops](https://github.com/globberwops))
- Suppress clang-analyzer-cplusplus.NewDeleteLeaks [\#444](https://github.com/doctest/doctest/pull/444) ([ncihnegn](https://github.com/ncihnegn))
- log contexts for failures in JUnit reporter [\#442](https://github.com/doctest/doctest/pull/442) ([runave](https://github.com/runave))
- Fix 32bit support on macOS [\#440](https://github.com/doctest/doctest/pull/440) ([AlexanderLanin](https://github.com/AlexanderLanin))

## [2.4.1](https://github.com/doctest/doctest/tree/2.4.1) (2020-11-04)
[Full Changelog](https://github.com/doctest/doctest/compare/2.4.0...2.4.1)

**Closed issues:**

- Avoid old C-style casts [\#424](https://github.com/doctest/doctest/issues/424)
- Segfault in unwind [\#422](https://github.com/doctest/doctest/issues/422)
- Inspect exception with gdb [\#421](https://github.com/doctest/doctest/issues/421)
- use-of-uninitialized-value [\#414](https://github.com/doctest/doctest/issues/414)
- Support unit tests with MPI [\#413](https://github.com/doctest/doctest/issues/413)
- Break into debugger support is missing for Linux [\#411](https://github.com/doctest/doctest/issues/411)
- What if built doctest as static library instead of header-only [\#408](https://github.com/doctest/doctest/issues/408)
- \[Question\] How to get test case name [\#407](https://github.com/doctest/doctest/issues/407)
- create extensions header for optional features requiring more std includes or newer C++ features [\#405](https://github.com/doctest/doctest/issues/405)
- tests/asserts summary lines are misaligned when counts exceed 999999 [\#402](https://github.com/doctest/doctest/issues/402)
- Call to 'ne' is ambiguous -- with solution [\#395](https://github.com/doctest/doctest/issues/395)
- Intermittent Segfaults [\#391](https://github.com/doctest/doctest/issues/391)
- Junit classname [\#390](https://github.com/doctest/doctest/issues/390)
- Add default printers for enums [\#121](https://github.com/doctest/doctest/issues/121)

**Merged pull requests:**

- Enum support \(fix for Issue \#121\) [\#429](https://github.com/doctest/doctest/pull/429) ([jkriegshauser](https://github.com/jkriegshauser))
- Support Clang 3.4 [\#428](https://github.com/doctest/doctest/pull/428) ([AlexanderLanin](https://github.com/AlexanderLanin))
- Silence remarks on old C-style casts [\#425](https://github.com/doctest/doctest/pull/425) ([UnePierre](https://github.com/UnePierre))
- Initial MPI unit tests implementation [\#418](https://github.com/doctest/doctest/pull/418) ([BerengerBerthoul](https://github.com/BerengerBerthoul))
- Add JUNIT\_OUTPUT\_DIR option to doctest\_discover\_tests [\#417](https://github.com/doctest/doctest/pull/417) ([Tradias](https://github.com/Tradias))
- Add option to build with std headers. [\#416](https://github.com/doctest/doctest/pull/416) ([avostrik](https://github.com/avostrik))
- Port Catch2 break into debugger for Linux. closes \#411 [\#412](https://github.com/doctest/doctest/pull/412) ([mikezackles](https://github.com/mikezackles))
- summary: align even large values \#402 [\#403](https://github.com/doctest/doctest/pull/403) ([dankamongmen](https://github.com/dankamongmen))
- Add breakpoint inline assembly for the Apple Silicon macOS. [\#400](https://github.com/doctest/doctest/pull/400) ([bruvzg](https://github.com/bruvzg))
- fix google's death test URI in roadmap [\#393](https://github.com/doctest/doctest/pull/393) ([ashutosh108](https://github.com/ashutosh108))

## [2.4.0](https://github.com/doctest/doctest/tree/2.4.0) (2020-06-27)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.8...2.4.0)

**Closed issues:**

- Count points based on the number of passed/failed cases? [\#386](https://github.com/doctest/doctest/issues/386)
- How to understand "\#data\_array" in std::string? [\#383](https://github.com/doctest/doctest/issues/383)
- crash: doctest with custom allocator [\#382](https://github.com/doctest/doctest/issues/382)
- Feature Request: format PRIVATE/PUBLIC/INTERFACE entries with constant indentation [\#378](https://github.com/doctest/doctest/issues/378)
- JUnit Reporter for Doctest [\#376](https://github.com/doctest/doctest/issues/376)
- Avoiding Feature Bloat [\#374](https://github.com/doctest/doctest/issues/374)
- StringMaker\<wchar\_t\> fail to compile with C++20 enabled \(GCC\) [\#357](https://github.com/doctest/doctest/issues/357)
- doctest\_discover\_tests and FetchContent\_Declare [\#351](https://github.com/doctest/doctest/issues/351)
- Junit reporter [\#318](https://github.com/doctest/doctest/issues/318)

**Merged pull requests:**

- Add a note that doctest can be installed through Homebrew [\#388](https://github.com/doctest/doctest/pull/388) ([cameronwhite](https://github.com/cameronwhite))
- provide alternative implementation of has\_insertion\_operator for C++20 [\#387](https://github.com/doctest/doctest/pull/387) ([lukaszgemborowski](https://github.com/lukaszgemborowski))
- Fix issue template to mention doctest [\#380](https://github.com/doctest/doctest/pull/380) ([nyanpasu64](https://github.com/nyanpasu64))

## [2.3.8](https://github.com/doctest/doctest/tree/2.3.8) (2020-05-17)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.7...2.3.8)

**Closed issues:**

- Scenario name can not be passed to -tc to execute single scenario [\#373](https://github.com/doctest/doctest/issues/373)
- Compile Error with CHECK\_NOTHROW when using 2 Template Arguments [\#372](https://github.com/doctest/doctest/issues/372)
- dll example won't compile [\#371](https://github.com/doctest/doctest/issues/371)
- Build error with MinGW \(Mingw-w64\) due to missing Windows.h \(with capital W\) [\#370](https://github.com/doctest/doctest/issues/370)
- How to override file\_line\_to\_stream? [\#369](https://github.com/doctest/doctest/issues/369)
- Memory sanitizer fails. [\#365](https://github.com/doctest/doctest/issues/365)
- Warning c6319 in Visual Studio [\#359](https://github.com/doctest/doctest/issues/359)
- Any option to show each test case's execute time? [\#358](https://github.com/doctest/doctest/issues/358)
- doctest in embedded [\#355](https://github.com/doctest/doctest/issues/355)
- Reloading a plugin with test cases leads to a segmentation fault [\#350](https://github.com/doctest/doctest/issues/350)
- Compiling with DOCTEST\_CONFIG\_COLORS\_ANSI fails on Windows [\#348](https://github.com/doctest/doctest/issues/348)
- Can I inherit ConsoleReporter? [\#344](https://github.com/doctest/doctest/issues/344)
- Noreturn and noexcept defines for Visual Studio 2013 support [\#327](https://github.com/doctest/doctest/issues/327)
- Data-driven testing -- print out the deepest DOCTEST\_SUBCASE [\#215](https://github.com/doctest/doctest/issues/215)
- Print the SUBCASE path when an assert fails in the TEST\_CASE body [\#125](https://github.com/doctest/doctest/issues/125)

**Merged pull requests:**

- fix: possible UB with nullptr increment [\#368](https://github.com/doctest/doctest/pull/368) ([oktonion](https://github.com/oktonion))
- Use CMake's CMP0077 policy if available [\#363](https://github.com/doctest/doctest/pull/363) ([thelink2012](https://github.com/thelink2012))
- Fix warning c6319 in Visual Studio 16.5 [\#361](https://github.com/doctest/doctest/pull/361) ([Cvelth](https://github.com/Cvelth))

## [2.3.7](https://github.com/doctest/doctest/tree/2.3.7) (2020-02-24)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.6...2.3.7)

**Closed issues:**

- Some of the GitHub CI builds are failing [\#334](https://github.com/doctest/doctest/issues/334)
- C++20 removed std::uncaught\_exception [\#333](https://github.com/doctest/doctest/issues/333)
- Doctest SEH handlers are called before \_\_except handlers [\#324](https://github.com/doctest/doctest/issues/324)

**Merged pull requests:**

- using std namespace where necessary and timer ticks fix [\#341](https://github.com/doctest/doctest/pull/341) ([oktonion](https://github.com/oktonion))
- fix std::uncaught\_exceptions [\#340](https://github.com/doctest/doctest/pull/340) ([cyyever](https://github.com/cyyever))
- Fix GitHub CI and add GitHub build badges [\#336](https://github.com/doctest/doctest/pull/336) ([claremacrae](https://github.com/claremacrae))
- http -\> https [\#331](https://github.com/doctest/doctest/pull/331) ([Coeur](https://github.com/Coeur))
- Switch to catching unhandled exceptions on Windows Closes \#324 [\#325](https://github.com/doctest/doctest/pull/325) ([jkriegshauser](https://github.com/jkriegshauser))

## [2.3.6](https://github.com/doctest/doctest/tree/2.3.6) (2019-12-16)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.5...2.3.6)

**Closed issues:**

- Link problem w/ BUILD=Release if MESSAGE\(\) with std::string/ostream-operator is used [\#316](https://github.com/doctest/doctest/issues/316)
- the FAQ about difference to Catch2 is missing tags [\#315](https://github.com/doctest/doctest/issues/315)
- include Windows.h in small caps to silence clang warnings [\#312](https://github.com/doctest/doctest/issues/312)
- Mistake in generator with lgtm error [\#311](https://github.com/doctest/doctest/issues/311)
- CMake: cannot install target doctest\_with\_main [\#310](https://github.com/doctest/doctest/issues/310)
- \[bug\] INFO\(\) and CAPTURE\(\) cannot compile using MSVC when used with DOCTEST\_CONFIG\_IMPLEMENTATION\_IN\_DLL [\#306](https://github.com/doctest/doctest/issues/306)
- Skip subcase [\#304](https://github.com/doctest/doctest/issues/304)
- Does some equivalent features from google test exist here? [\#300](https://github.com/doctest/doctest/issues/300)
- How to use doctest in dll only\(without main.cpp and .exe\) [\#299](https://github.com/doctest/doctest/issues/299)
- Warning: C26812: The enum type 'doctest::assertType::Enum' is unscoped. Prefer 'enum class' over 'enum' \(Enum.3\). [\#298](https://github.com/doctest/doctest/issues/298)
- test executable\_dll\_and\_plugin fails on Linux, GCC 8.1.0, -fsanitize=address [\#201](https://github.com/doctest/doctest/issues/201)

**Merged pull requests:**

- Fixed missing ostream include for MacOS when defining DOCTEST\_CONFIG\_… [\#314](https://github.com/doctest/doctest/pull/314) ([NKTomHaygarth](https://github.com/NKTomHaygarth))
- include windows.h in cmall caps to silence clang nonportable warnings [\#313](https://github.com/doctest/doctest/pull/313) ([suoniq](https://github.com/suoniq))
- Add .editorconfig file. [\#301](https://github.com/doctest/doctest/pull/301) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Add Github Actions CI [\#285](https://github.com/doctest/doctest/pull/285) ([DaanDeMeyer](https://github.com/DaanDeMeyer))

## [2.3.5](https://github.com/doctest/doctest/tree/2.3.5) (2019-09-22)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.4...2.3.5)

**Closed issues:**

- \[feature request\] Assertion macros for throwing exception of a specific type with message - \<LEVEL\>\_THROWS\_WITH\_AS\(expr, string, ex\_type\) [\#295](https://github.com/doctest/doctest/issues/295)
- CHECK\_THROWS\_AS of non-default constructor wants to call default constructor [\#293](https://github.com/doctest/doctest/issues/293)
- Typos and spelling errors in source, documentation and scripts [\#291](https://github.com/doctest/doctest/issues/291)
- Customize test names / variable substitution [\#284](https://github.com/doctest/doctest/issues/284)
- SUBCASE in function not behaving as expected [\#282](https://github.com/doctest/doctest/issues/282)
- SUPER\_FAST\_ASSERTS fails to compile CHECK\_MESSAGE [\#281](https://github.com/doctest/doctest/issues/281)
- CHECK\_MESSAGE no longer works with DOCTEST\_CONFIG\_SUPER\_FAST\_ASSERTS [\#280](https://github.com/doctest/doctest/issues/280)
- CAPTURE of structured binding element no longer works [\#279](https://github.com/doctest/doctest/issues/279)
- Reporter: `test\_case\_end` no longer fired after test case restart [\#278](https://github.com/doctest/doctest/issues/278)
- Add debug break override support [\#277](https://github.com/doctest/doctest/issues/277)
- Running tests from within Visual Studio in a static lib project [\#275](https://github.com/doctest/doctest/issues/275)
- Compile-time error when using a raw string literal inside of REQUIRE \(MSVC 2017\) [\#274](https://github.com/doctest/doctest/issues/274)
- Give example for having tests in production code [\#252](https://github.com/doctest/doctest/issues/252)
- Memory leaks just by including doctest.h [\#205](https://github.com/doctest/doctest/issues/205)
- Feature request: print subcase when an exception is thrown inside one [\#136](https://github.com/doctest/doctest/issues/136)

**Merged pull requests:**

- Fix typos and misspellings found by codespell. [\#292](https://github.com/doctest/doctest/pull/292) ([warmsocks](https://github.com/warmsocks))
- Document order by issue correctly [\#290](https://github.com/doctest/doctest/pull/290) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Document that -order-by=file is compiler-dependent [\#289](https://github.com/doctest/doctest/pull/289) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Add -order-by=name to filter\_2 test [\#288](https://github.com/doctest/doctest/pull/288) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Add support for compiling with clang-cl [\#286](https://github.com/doctest/doctest/pull/286) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- No minimum version limitation of Meson [\#283](https://github.com/doctest/doctest/pull/283) ([ydm](https://github.com/ydm))

## [2.3.4](https://github.com/doctest/doctest/tree/2.3.4) (2019-08-12)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.3...2.3.4)

**Closed issues:**

- Remove INFO\(\) limitation for using only lvalues and no rvalues [\#269](https://github.com/doctest/doctest/issues/269)
- Compile error on MAC OS with AppleClang 8.0.0.8000042  [\#266](https://github.com/doctest/doctest/issues/266)
- Throwing exception in a mocked method [\#265](https://github.com/doctest/doctest/issues/265)
- Illegal syntax for decorators compiles and runs without warning, but has no effect [\#264](https://github.com/doctest/doctest/issues/264)
- Support conditional expressions in REQUIRE [\#262](https://github.com/doctest/doctest/issues/262)
- Register a listener\(reporter\) that always listens [\#257](https://github.com/doctest/doctest/issues/257)
- Memory sanitizer complaint [\#255](https://github.com/doctest/doctest/issues/255)
- Windows Clang GNU command line warnings [\#253](https://github.com/doctest/doctest/issues/253)
- The build writes into the source directory [\#249](https://github.com/doctest/doctest/issues/249)
- How to enable tests inside another exe [\#246](https://github.com/doctest/doctest/issues/246)
- Testing multiple headers. [\#244](https://github.com/doctest/doctest/issues/244)
- CMakeLists.txt: Needs CMAKE\_CXX\_STANDARD=11 [\#243](https://github.com/doctest/doctest/issues/243)
- \[bug\] Can't compile the tests because of mutex, that is declared in the doctest [\#242](https://github.com/doctest/doctest/issues/242)

**Merged pull requests:**

- Improve Listener docs [\#273](https://github.com/doctest/doctest/pull/273) ([claremacrae](https://github.com/claremacrae))
- Rework `INFO` lazy evaluation to use lambdas. [\#270](https://github.com/doctest/doctest/pull/270) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Prevent compile errors with AppleClang compiler [\#268](https://github.com/doctest/doctest/pull/268) ([ClausKlein](https://github.com/ClausKlein))
- Revert "fix : including windows.h header cause error" [\#263](https://github.com/doctest/doctest/pull/263) ([onqtam](https://github.com/onqtam))
- Fix static analyzer URLs [\#259](https://github.com/doctest/doctest/pull/259) ([godbyk](https://github.com/godbyk))
- fix : including windows.h header cause error [\#258](https://github.com/doctest/doctest/pull/258) ([rinechran](https://github.com/rinechran))
- only look for C++ compiler with CMake [\#256](https://github.com/doctest/doctest/pull/256) ([zhihaoy](https://github.com/zhihaoy))
- Fix \#253 [\#254](https://github.com/doctest/doctest/pull/254) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- add alias target for doctest for use in build tree [\#247](https://github.com/doctest/doctest/pull/247) ([trondhe](https://github.com/trondhe))

## [2.3.3](https://github.com/doctest/doctest/tree/2.3.3) (2019-06-02)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.2...2.3.3)

**Closed issues:**

- Build fails with gcc9 because of -Wstrict-overflow=5 which is too high [\#241](https://github.com/doctest/doctest/issues/241)
- doctest given defined with short macro name [\#239](https://github.com/doctest/doctest/issues/239)
- Splitting templated test across different translation units [\#238](https://github.com/doctest/doctest/issues/238)
- Compile errors with iosfwd.h and Visual Studio 2019 Preview [\#183](https://github.com/doctest/doctest/issues/183)
- Add CMake test support as catch\_discover\_tests\(\) in Catch2 [\#171](https://github.com/doctest/doctest/issues/171)

**Merged pull requests:**

- fix \#239 - use long macro name [\#240](https://github.com/doctest/doctest/pull/240) ([m-bd](https://github.com/m-bd))
- Add doctest\_discover\_tests\(\) [\#236](https://github.com/doctest/doctest/pull/236) ([reddwarf69](https://github.com/reddwarf69))
- Ignore redundant-decls warning on MinGW [\#235](https://github.com/doctest/doctest/pull/235) ([AMS21](https://github.com/AMS21))
- Fixed meson build file dependency declaration [\#233](https://github.com/doctest/doctest/pull/233) ([jormundgand](https://github.com/jormundgand))

## [2.3.2](https://github.com/doctest/doctest/tree/2.3.2) (2019-05-06)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.1...2.3.2)

**Closed issues:**

- scripts/bench/run\_all.py : module 'urllib' has no attribute 'urlretrieve' [\#230](https://github.com/doctest/doctest/issues/230)
- wrong set of tests registered with TEST\_CASE\_TEMPLATE get executed [\#228](https://github.com/doctest/doctest/issues/228)
- Logging not Working for me [\#227](https://github.com/doctest/doctest/issues/227)
- Link test runner executable into dll? [\#226](https://github.com/doctest/doctest/issues/226)
- Linking issue for executables after including doctest in library [\#224](https://github.com/doctest/doctest/issues/224)
- Strange REQUIRE\_THROWS behaviour [\#223](https://github.com/doctest/doctest/issues/223)
- Windows clang-cl -Wunused-variable warning [\#221](https://github.com/doctest/doctest/issues/221)
- Update doctest 2.3.1 in bincrafters [\#220](https://github.com/doctest/doctest/issues/220)
- make install, on 64 bit, installs cmake files into lib instead of lib64 folder  [\#218](https://github.com/doctest/doctest/issues/218)
- TSAN: data race related to hasLoggedCurrentTestStart [\#217](https://github.com/doctest/doctest/issues/217)
- REQUIRE\_THROWS\_AS does not support class constructors [\#216](https://github.com/doctest/doctest/issues/216)
- Build failure on clang 7.0.1 on Fedora 29 [\#214](https://github.com/doctest/doctest/issues/214)
- add example compatible with -\> https://github.com/report-ci/ [\#212](https://github.com/doctest/doctest/issues/212)
- No DOCTEST\_WITH\_TESTS? [\#211](https://github.com/doctest/doctest/issues/211)

**Merged pull requests:**

- Added meson file, to declare a dependency. [\#232](https://github.com/doctest/doctest/pull/232) ([jormundgand](https://github.com/jormundgand))
- Explicitly specify the doctest\_with\_main C++ standard in CMake. [\#231](https://github.com/doctest/doctest/pull/231) ([DaanDeMeyer](https://github.com/DaanDeMeyer))
- Remove architecture check from CMake package [\#225](https://github.com/doctest/doctest/pull/225) ([mmha](https://github.com/mmha))
- add default install prefix [\#219](https://github.com/doctest/doctest/pull/219) ([a4z](https://github.com/a4z))
- \[regression\] Workaround MSVC preprocessor issue triggered by REQUIRE\_THROWS [\#213](https://github.com/doctest/doctest/pull/213) ([zhihaoy](https://github.com/zhihaoy))

## [2.3.1](https://github.com/doctest/doctest/tree/2.3.1) (2019-03-24)
[Full Changelog](https://github.com/doctest/doctest/compare/2.3.0...2.3.1)

**Merged pull requests:**

- Add two very simple examples of using doctest with CMake [\#209](https://github.com/doctest/doctest/pull/209) ([pr0g](https://github.com/pr0g))

## [2.3.0](https://github.com/doctest/doctest/tree/2.3.0) (2019-03-23)
[Full Changelog](https://github.com/doctest/doctest/compare/2.2.3...2.3.0)

**Closed issues:**

- Compilation with emscripten fails by default because of signal handling [\#207](https://github.com/doctest/doctest/issues/207)
- Compilation fails with cl.exe /Zc:wchar\_t- [\#206](https://github.com/doctest/doctest/issues/206)
- Parallel invocation of doctest's own testsuite via CTest fails [\#202](https://github.com/doctest/doctest/issues/202)
-  Get the number of passed/failed tests in the code [\#200](https://github.com/doctest/doctest/issues/200)
- Tests alongside code with multiple executables [\#199](https://github.com/doctest/doctest/issues/199)
- Cppcheck 1.86 warnings [\#198](https://github.com/doctest/doctest/issues/198)
- Compiling as Dll maybe is wrong [\#196](https://github.com/doctest/doctest/issues/196)
- Forward-declaring identifiers in std:: is UB - consider including some of the cheaper C/C++ stdlib headers [\#194](https://github.com/doctest/doctest/issues/194)
- QtCreator + clang warning about operator \<\< precedence [\#191](https://github.com/doctest/doctest/issues/191)
- run test fixture from cli [\#190](https://github.com/doctest/doctest/issues/190)
- Installing doctest using cmake and make fails on Ubuntu 16.04 \(C++11 is not used\) [\#189](https://github.com/doctest/doctest/issues/189)
- c++17 requirement for testing private members [\#188](https://github.com/doctest/doctest/issues/188)
- \[feature request\] implement a user-extendable reporter system [\#138](https://github.com/doctest/doctest/issues/138)
- Same test runs multiple times when written in a header and included with different unnormalized paths [\#45](https://github.com/doctest/doctest/issues/45)

**Merged pull requests:**

- Fix unmatched bracket in DOCTEST\_TEST\_CASE\_CLASS [\#204](https://github.com/doctest/doctest/pull/204) ([patstew](https://github.com/patstew))
- Template apply [\#203](https://github.com/doctest/doctest/pull/203) ([zhihaoy](https://github.com/zhihaoy))
- No undefined behavior per C++ standard in detecting endianness. [\#195](https://github.com/doctest/doctest/pull/195) ([dimztimz](https://github.com/dimztimz))
- Fix propagating include directories of target doctest\_with\_main [\#193](https://github.com/doctest/doctest/pull/193) ([dimztimz](https://github.com/dimztimz))
-  Move single header to a separate folder [\#187](https://github.com/doctest/doctest/pull/187) ([dimztimz](https://github.com/dimztimz))
- Fix Clang format to handle C++11 [\#186](https://github.com/doctest/doctest/pull/186) ([dimztimz](https://github.com/dimztimz))
- Rename doctest\_impl.h to doctest.cpp for less confusion. [\#185](https://github.com/doctest/doctest/pull/185) ([dimztimz](https://github.com/dimztimz))

## [2.2.3](https://github.com/doctest/doctest/tree/2.2.3) (2019-02-10)
[Full Changelog](https://github.com/doctest/doctest/compare/2.2.2...2.2.3)

**Closed issues:**

- Calling convention needed on a few functions [\#182](https://github.com/doctest/doctest/issues/182)
- Terminal color is not reset when a test fails with some signal [\#122](https://github.com/doctest/doctest/issues/122)

## [2.2.2](https://github.com/doctest/doctest/tree/2.2.2) (2019-01-28)
[Full Changelog](https://github.com/doctest/doctest/compare/2.2.1...2.2.2)

**Closed issues:**

- Add way to override getCurrentTicks\(\) implementation [\#178](https://github.com/doctest/doctest/issues/178)
- Wrap \<csignal\> include with ifdef [\#177](https://github.com/doctest/doctest/issues/177)
- How to stop doctest hijack unhandled exceptions? [\#176](https://github.com/doctest/doctest/issues/176)
- Change the include path of the `doctest` CMake interface target so users need to specify the folder as well [\#175](https://github.com/doctest/doctest/issues/175)
- Reduce scope of DebugOutputWindowReporter instance [\#174](https://github.com/doctest/doctest/issues/174)
- Can logging \(INFO\) be used in helper class outside of TEST\_CASE? [\#169](https://github.com/doctest/doctest/issues/169)

**Merged pull requests:**

- Change the include path in examples as \#175 [\#180](https://github.com/doctest/doctest/pull/180) ([ncihnegn](https://github.com/ncihnegn))
- Fix CMake include path \#175 [\#179](https://github.com/doctest/doctest/pull/179) ([ncihnegn](https://github.com/ncihnegn))

## [2.2.1](https://github.com/doctest/doctest/tree/2.2.1) (2019-01-15)
[Full Changelog](https://github.com/doctest/doctest/compare/2.2.0...2.2.1)

**Closed issues:**

- the `--no-throw` option shouldn't affect `\<LEVEL\>\_NOTHROW` asserts [\#173](https://github.com/doctest/doctest/issues/173)
- Make doctest work with XCode 6 and 7 \(no support for C++11 thread\_local\) [\#172](https://github.com/doctest/doctest/issues/172)
- Print vector content. [\#170](https://github.com/doctest/doctest/issues/170)
- Conan package [\#103](https://github.com/doctest/doctest/issues/103)
- \[feature request\] Thread-safety for asserts and logging facilities [\#4](https://github.com/doctest/doctest/issues/4)

## [2.2.0](https://github.com/doctest/doctest/tree/2.2.0) (2018-12-05)
[Full Changelog](https://github.com/doctest/doctest/compare/2.1.0...2.2.0)

**Closed issues:**

- remove the FAST\_ versions of the binary asserts \(not a breaking change!\) [\#167](https://github.com/doctest/doctest/issues/167)
- \[compile times\] make the DOCTEST\_CONFIG\_SUPER\_FAST\_ASSERTS identifier affect normal asserts too [\#166](https://github.com/doctest/doctest/issues/166)

## [2.1.0](https://github.com/doctest/doctest/tree/2.1.0) (2018-11-30)
[Full Changelog](https://github.com/doctest/doctest/compare/2.0.1...2.1.0)

**Closed issues:**

- doctest::String ctor with non-zero terminated string [\#165](https://github.com/doctest/doctest/issues/165)
- thread\_local is not supported on iOS 9.0 [\#164](https://github.com/doctest/doctest/issues/164)
- Compiler error on Android NDK r18 [\#163](https://github.com/doctest/doctest/issues/163)
- \[question\] One setup for multiple tests [\#160](https://github.com/doctest/doctest/issues/160)
- clang unwanted warning in user code [\#156](https://github.com/doctest/doctest/issues/156)
- Unsigned integer overflow in fileOrderComparator [\#151](https://github.com/doctest/doctest/issues/151)
- ThreadSanitizer: signal-unsafe call inside of a signal [\#147](https://github.com/doctest/doctest/issues/147)
- Feature request: check for exception string \(like Catch's CHECK\_THROWS\_WITH\) [\#97](https://github.com/doctest/doctest/issues/97)

**Merged pull requests:**

- Fixed build error under Android NDK [\#162](https://github.com/doctest/doctest/pull/162) ([tals](https://github.com/tals))
- Added clang-7 to travis build [\#161](https://github.com/doctest/doctest/pull/161) ([AMS21](https://github.com/AMS21))
- Remove clang-tidy warnings for static fields created by doctest [\#159](https://github.com/doctest/doctest/pull/159) ([rantasub](https://github.com/rantasub))
- Make it possible to change the command line options prefix [\#158](https://github.com/doctest/doctest/pull/158) ([tbleher](https://github.com/tbleher))

## [2.0.1](https://github.com/doctest/doctest/tree/2.0.1) (2018-10-24)
[Full Changelog](https://github.com/doctest/doctest/compare/2.0.0...2.0.1)

**Closed issues:**

- macro name collision with google log [\#157](https://github.com/doctest/doctest/issues/157)
- Add \#define to not run tests by default [\#152](https://github.com/doctest/doctest/issues/152)
- REQUIRE\_THROWS\_MESSAGE not checking message correctly [\#150](https://github.com/doctest/doctest/issues/150)
- Test case passes even though subcase failed [\#149](https://github.com/doctest/doctest/issues/149)

**Merged pull requests:**

- Correctly document when a main\(\) entry point will be created [\#155](https://github.com/doctest/doctest/pull/155) ([tbleher](https://github.com/tbleher))
- Correct format string for unsigned char [\#154](https://github.com/doctest/doctest/pull/154) ([tbleher](https://github.com/tbleher))

## [2.0.0](https://github.com/doctest/doctest/tree/2.0.0) (2018-08-23)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.9...2.0.0)

**Closed issues:**

- MSVC 2017 15.8.1, New Warnings as Errors [\#144](https://github.com/doctest/doctest/issues/144)
- Windows clang-cl -Wdeprecated-declarations warnings [\#143](https://github.com/doctest/doctest/issues/143)
- Logo Proposal for Doctest [\#141](https://github.com/doctest/doctest/issues/141)
- PCH Support [\#140](https://github.com/doctest/doctest/issues/140)
- improve compile times even further [\#139](https://github.com/doctest/doctest/issues/139)
- !!! BREAKING CHANGE !!! - Move to C++11 for next version of the library [\#137](https://github.com/doctest/doctest/issues/137)
- getCurrentTicks producing warning on MinGW [\#133](https://github.com/doctest/doctest/issues/133)
- \[enhancement\] Add support for "stand-alone assertions". [\#114](https://github.com/doctest/doctest/issues/114)

**Merged pull requests:**

- Suppress compiler warning on MinGW [\#134](https://github.com/doctest/doctest/pull/134) ([AMS21](https://github.com/AMS21))

## [1.2.9](https://github.com/doctest/doctest/tree/1.2.9) (2018-05-10)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.8...1.2.9)

**Closed issues:**

- GCC 8.0 std::uncaught\_exception\(\) is deprecated  [\#130](https://github.com/doctest/doctest/issues/130)
- Signal stack size too small on Linux [\#129](https://github.com/doctest/doctest/issues/129)
- Support Intel Compiler [\#128](https://github.com/doctest/doctest/issues/128)
- Please add support for MSVC 2005 [\#127](https://github.com/doctest/doctest/issues/127)
- scan-build report "Dereference of null pointer" for function wildcmp [\#124](https://github.com/doctest/doctest/issues/124)
- !!! BREAKING CHANGE \(console output only\)  !!! - Emulate the error/warning format emitted by native compiler gcc/clang/msvc when printing test failures in the log [\#123](https://github.com/doctest/doctest/issues/123)
- ARM builds: FTBFS on armhf - error: cast from 'const char\*' to 'const  [\#118](https://github.com/doctest/doctest/issues/118)

**Merged pull requests:**

- Exclude Intel from GCC compiler check [\#132](https://github.com/doctest/doctest/pull/132) ([smcallis](https://github.com/smcallis))
- Fix deprecated-declarations warning with GCC-8.0 [\#131](https://github.com/doctest/doctest/pull/131) ([AMS21](https://github.com/AMS21))

## [1.2.8](https://github.com/doctest/doctest/tree/1.2.8) (2018-03-10)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.7...1.2.8)

**Closed issues:**

- ARM64 builds: templated\_test\_cases.cpp test fails [\#119](https://github.com/doctest/doctest/issues/119)

## [1.2.7](https://github.com/doctest/doctest/tree/1.2.7) (2018-02-06)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.6...1.2.7)

**Closed issues:**

- MSan has runtime error: unsigned integer overflow [\#116](https://github.com/doctest/doctest/issues/116)
- clang-tidy warning about cert-err58-cpp [\#115](https://github.com/doctest/doctest/issues/115)
- Linking errors [\#113](https://github.com/doctest/doctest/issues/113)
- inlining function defs [\#111](https://github.com/doctest/doctest/issues/111)
- Nullptr issue. [\#110](https://github.com/doctest/doctest/issues/110)
- MemorySanitizer: use-of-uninitialized-value [\#109](https://github.com/doctest/doctest/issues/109)
- Potential memory leak through scan-build [\#108](https://github.com/doctest/doctest/issues/108)
- Warnings raised to error with latest MSVC version [\#107](https://github.com/doctest/doctest/issues/107)
- New solution for tests in static libraries ! \(MSVC\) [\#106](https://github.com/doctest/doctest/issues/106)
- Command line flags do not work after code formatter/beautifier [\#104](https://github.com/doctest/doctest/issues/104)
- Cppcheck 1.81 warnings [\#102](https://github.com/doctest/doctest/issues/102)

**Merged pull requests:**

- Fix macros WIN32\_LEAN\_AND\_MEAN typo [\#112](https://github.com/doctest/doctest/pull/112) ([vladimirgamalyan](https://github.com/vladimirgamalyan))
- Correct DOCTEST\_NO\_INSTALL logic; do install unless it is set \(\#99\) [\#100](https://github.com/doctest/doctest/pull/100) ([onqtam](https://github.com/onqtam))
- Correct DOCTEST\_NO\_INSTALL logic; do install unless it is set [\#99](https://github.com/doctest/doctest/pull/99) ([OdyX](https://github.com/OdyX))

## [1.2.6](https://github.com/doctest/doctest/tree/1.2.6) (2017-10-29)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.5...1.2.6)

**Closed issues:**

- \[bug\] writing an exception translator in a header file results in it being registered multiple times which is suboptimal [\#98](https://github.com/doctest/doctest/issues/98)
- Warnings when using something more than /W4 for Visual Studio [\#95](https://github.com/doctest/doctest/issues/95)

**Merged pull requests:**

- Added an option to not install Doctest in the CMake scripts [\#96](https://github.com/doctest/doctest/pull/96) ([nm17](https://github.com/nm17))
- Adding a defensive check against a null pointer for the current test suite [\#94](https://github.com/doctest/doctest/pull/94) ([Lectem](https://github.com/Lectem))
- Remove incomplete copy ctor [\#93](https://github.com/doctest/doctest/pull/93) ([McMartin](https://github.com/McMartin))

## [1.2.5](https://github.com/doctest/doctest/tree/1.2.5) (2017-10-06)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.4...1.2.5)

**Closed issues:**

- Xcode 9 / clang - unknown warning group [\#92](https://github.com/doctest/doctest/issues/92)

## [1.2.4](https://github.com/doctest/doctest/tree/1.2.4) (2017-09-20)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.3...1.2.4)

**Closed issues:**

- \[bug\] test cases can end up in the wrong test suite [\#91](https://github.com/doctest/doctest/issues/91)

## [1.2.3](https://github.com/doctest/doctest/tree/1.2.3) (2017-09-11)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.2...1.2.3)

**Closed issues:**

- \[bug\] Defining a variable T inside a test with DOCTEST\_CONFIG\_DISABLE defined does not compile [\#90](https://github.com/doctest/doctest/issues/90)
- \[support\] Using `DOCTEST\_CONFIG\_NO\_SHORT\_MACRO\_NAMES` does not compile using g++ 6.3.0 [\#89](https://github.com/doctest/doctest/issues/89)
- \[question\] Why are SUBCASEs executed only once when within a function called multiple times? [\#88](https://github.com/doctest/doctest/issues/88)

## [1.2.2](https://github.com/doctest/doctest/tree/1.2.2) (2017-09-05)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.1...1.2.2)

**Closed issues:**

- \[question\] Differences between doctest and googletest \(gtest\) for uninitialised local variables in test cases [\#86](https://github.com/doctest/doctest/issues/86)
- !!! BREAKING CHANGE !!! - remove the custom implementation of std::is\_constructible and optionally use the \<type\_traits\> header because of infinite template recursion issues with GCC [\#85](https://github.com/doctest/doctest/issues/85)
- Static Analysis results of doctest [\#83](https://github.com/doctest/doctest/issues/83)
- !!! BREAKING CHANGE !!! - catch exceptions as const reference in \<LEVEL\>\_THROWS\_AS [\#81](https://github.com/doctest/doctest/issues/81)
- doctest implementation as static library [\#77](https://github.com/doctest/doctest/issues/77)
- Provide some easy way to compare structs containing float/doubles [\#73](https://github.com/doctest/doctest/issues/73)

**Merged pull requests:**

- Add support for templated scenarios [\#87](https://github.com/doctest/doctest/pull/87) ([Lectem](https://github.com/Lectem))
- Prefer if\(MSVC\) in CMakeLists.txt. [\#84](https://github.com/doctest/doctest/pull/84) ([martinmoene](https://github.com/martinmoene))
- catch throw\_as exception as const reference [\#82](https://github.com/doctest/doctest/pull/82) ([a4z](https://github.com/a4z))
- Added doctest\_with\_main static lib [\#78](https://github.com/doctest/doctest/pull/78) ([ymadzhunkov](https://github.com/ymadzhunkov))

## [1.2.1](https://github.com/doctest/doctest/tree/1.2.1) (2017-05-24)
[Full Changelog](https://github.com/doctest/doctest/compare/1.2.0...1.2.1)

**Closed issues:**

- Compile error under MSVC 2015/2017 if \<thread\> included in same file as "doctest.h" [\#72](https://github.com/doctest/doctest/issues/72)

**Merged pull requests:**

- docs: TEST\_CASE\_METHOD -\> TEST\_CASE\_FIXTURE [\#71](https://github.com/doctest/doctest/pull/71) ([akrzemi1](https://github.com/akrzemi1))

## [1.2.0](https://github.com/doctest/doctest/tree/1.2.0) (2017-05-15)
[Full Changelog](https://github.com/doctest/doctest/compare/1.1.4...1.2.0)

**Closed issues:**

- Further improvements on compile time - disable inlining of functions used in asserts [\#70](https://github.com/doctest/doctest/issues/70)
- Improve runtime performance - lazy stringification, more inlining, no statics on the hot path, move semantics for classes such as doctest::String which are used by value, etc. [\#69](https://github.com/doctest/doctest/issues/69)
- Add option to show duration of test case execution and add a timeout\(seconds\) decorator - marking them as failed if they exceed it [\#68](https://github.com/doctest/doctest/issues/68)
- Add support for test case decorators - label, description, skip, may\_fail, should\_fail, expected\_failures, etc. [\#67](https://github.com/doctest/doctest/issues/67)
- Integrate static analysis into the CI builds [\#66](https://github.com/doctest/doctest/issues/66)
- Print the test suite name on test case failure [\#65](https://github.com/doctest/doctest/issues/65)
- Add signal handlers to handle crashes \(and use SEH under Windows\) - report which test case failed [\#63](https://github.com/doctest/doctest/issues/63)
- Add support to Approx for strong typedefs of double [\#62](https://github.com/doctest/doctest/issues/62)
- \[question\] Is there a way to always have 0 as the exit code regardless of test results? [\#59](https://github.com/doctest/doctest/issues/59)
- Add support for un-parenthesized expressions containing commas in asserts [\#58](https://github.com/doctest/doctest/issues/58)
- Add ability to filter subcases with filters [\#57](https://github.com/doctest/doctest/issues/57)
- Add option to query if code is being ran inside of a test - doctest::is\_running\_in\_test [\#56](https://github.com/doctest/doctest/issues/56)
- Ability for a binary \(executable / shared object\) to use the test runner implementation of another binary - with exported symbols - so tests end up in a single registry [\#55](https://github.com/doctest/doctest/issues/55)
- How to force the use of colors in the terminal? [\#54](https://github.com/doctest/doctest/issues/54)
- How can I mix production code with the Unit Tests? [\#53](https://github.com/doctest/doctest/issues/53)
- add \<= and \>= operators to Approx \(and also maybe \< and \>\) [\#52](https://github.com/doctest/doctest/issues/52)
- Add ability to capture variables from test scope [\#48](https://github.com/doctest/doctest/issues/48)
- !!! BREAKING CHANGE !!! - Make TEST\_SUITE work with blocks and add TEST\_SUITE\_BEGIN [\#41](https://github.com/doctest/doctest/issues/41)
- Add option to print which test suites/cases are run [\#39](https://github.com/doctest/doctest/issues/39)
- Add support for templated test cases - parameterized by type [\#38](https://github.com/doctest/doctest/issues/38)
- Add custom failure messages with lazy stringification [\#23](https://github.com/doctest/doctest/issues/23)
- Add an exception translation mechanism + the ability for users to extend it with custom exception types [\#12](https://github.com/doctest/doctest/issues/12)
- Add API for reporting failures [\#9](https://github.com/doctest/doctest/issues/9)

**Merged pull requests:**

- Update doctest to work with ARM DS5-compiler [\#64](https://github.com/doctest/doctest/pull/64) ([tomasnilefrost](https://github.com/tomasnilefrost))

## [1.1.4](https://github.com/doctest/doctest/tree/1.1.4) (2017-02-18)
[Full Changelog](https://github.com/doctest/doctest/compare/1.1.3...1.1.4)

**Closed issues:**

- Add option --force-colors - for when a tty is not detected for stdout [\#51](https://github.com/doctest/doctest/issues/51)
- Issue with using lambdas in tests in gcc [\#49](https://github.com/doctest/doctest/issues/49)
- Add the include file to releases [\#47](https://github.com/doctest/doctest/issues/47)

**Merged pull requests:**

- Add translation of std::exception for exceptions that terminate a test case [\#46](https://github.com/doctest/doctest/pull/46) ([eliaskosunen](https://github.com/eliaskosunen))

## [1.1.3](https://github.com/doctest/doctest/tree/1.1.3) (2016-11-15)
[Full Changelog](https://github.com/doctest/doctest/compare/1.1.2...1.1.3)

**Closed issues:**

- Exception handlers cause warnings when exceptions are disabled [\#44](https://github.com/doctest/doctest/issues/44)

## [1.1.2](https://github.com/doctest/doctest/tree/1.1.2) (2016-10-10)
[Full Changelog](https://github.com/doctest/doctest/compare/1.1.1...1.1.2)

**Closed issues:**

- clang warnings when using C++11 or newer [\#42](https://github.com/doctest/doctest/issues/42)
- \[support\] identical names for test suites? [\#40](https://github.com/doctest/doctest/issues/40)

## [1.1.1](https://github.com/doctest/doctest/tree/1.1.1) (2016-09-22)
[Full Changelog](https://github.com/doctest/doctest/compare/1.1.0...1.1.1)

## [1.1.0](https://github.com/doctest/doctest/tree/1.1.0) (2016-09-21)
[Full Changelog](https://github.com/doctest/doctest/compare/1.0.0...1.1.0)

**Closed issues:**

- char\* comparison uses the contents, not the pointer [\#36](https://github.com/doctest/doctest/issues/36)
- add configuration preprocessor identifier for passing by value in assertions instead of by reference [\#35](https://github.com/doctest/doctest/issues/35)
- restrict expressions in assertion macros to binary comparisons at most with a static assert [\#34](https://github.com/doctest/doctest/issues/34)
- Add clearFilters\(\) to doctest::Context [\#33](https://github.com/doctest/doctest/issues/33)
- A way to refrain from polluting “\#define” space for users of tested code? [\#32](https://github.com/doctest/doctest/issues/32)
- drop VC++6 support [\#31](https://github.com/doctest/doctest/issues/31)
- False positive test [\#30](https://github.com/doctest/doctest/issues/30)
- Turn off coloring after tests are finished? [\#28](https://github.com/doctest/doctest/issues/28)
- C++11 nullptr [\#27](https://github.com/doctest/doctest/issues/27)
- Only one SUBCASE per line is executed [\#25](https://github.com/doctest/doctest/issues/25)
- creative formatting of chars [\#24](https://github.com/doctest/doctest/issues/24)
- DOCTEST\_BREAK\_INTO\_DEBUGGER undefined under OSX [\#22](https://github.com/doctest/doctest/issues/22)
- Tests inside a static library [\#21](https://github.com/doctest/doctest/issues/21)
- Add example how to remove doctest options from the command line for the program after the tests run [\#20](https://github.com/doctest/doctest/issues/20)
- Single-letter options active even without leading '-' \(dash\) [\#19](https://github.com/doctest/doctest/issues/19)
- pointer stringification not working for compilers different from MSVC [\#18](https://github.com/doctest/doctest/issues/18)
- Tests that accompany code run and produce output at default [\#17](https://github.com/doctest/doctest/issues/17)
- GCC 5.3.1 Compiler warning: sign compare [\#16](https://github.com/doctest/doctest/issues/16)
- Slower than Catch in realistic test cases [\#14](https://github.com/doctest/doctest/issues/14)
- Rename doctest::detail::Result res; in DOCTEST\_ASSERT\_IMPLEMENT [\#10](https://github.com/doctest/doctest/issues/10)
- No red when all tests pass [\#7](https://github.com/doctest/doctest/issues/7)
- UNIX line feedings on GitHub please [\#6](https://github.com/doctest/doctest/issues/6)

**Merged pull requests:**

- don't show green when tests fail [\#26](https://github.com/doctest/doctest/pull/26) ([ferkulat](https://github.com/ferkulat))
- Include "program code" in example [\#15](https://github.com/doctest/doctest/pull/15) ([martinmoene](https://github.com/martinmoene))

## [1.0.0](https://github.com/doctest/doctest/tree/1.0.0) (2016-05-22)
**Merged pull requests:**

- Reduce the header size for test users [\#3](https://github.com/doctest/doctest/pull/3) ([zah](https://github.com/zah))
- Add a Gitter chat badge to README.md [\#1](https://github.com/doctest/doctest/pull/1) ([gitter-badger](https://github.com/gitter-badger))



\* *This Change Log was automatically generated by [github_changelog_generator](https://github.com/skywinder/Github-Changelog-Generator)*