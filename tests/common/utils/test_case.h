/**
 * @file utils/test_case.h
 * @brief High-level manipulation of the active test-case
 */

#include <doctest/parts/private/context_state.h>
#include <utils/subprocess.h>
#include <utils/executable.h>
#include <string>
#include <vector>

namespace utils {

/** Identifier for a test-case */
class test_case
{
public:
  class options {
  private:
    bool _junit              = false; ///< --reporters=junit
    bool _xml                = false; ///< --reporters=xml
    bool _success            = false; ///< --success
    bool _no_intro           = true;  ///< --no-intro
    bool _no_version         = true;  ///< --no-version
    bool _no_colors          = true;  ///< --no-colors
    bool _no_path_filenames  = true;  ///< --no-path-filenames
    bool _no_skipped_summary = true;  ///< --no-skipped-summary
    bool _no_time_in_output  = true;  ///< --no-time-in-output
    bool _gnu_file_line      = true;  ///< --gnu-file-line

  public:
    /** Default-constructor */
    inline options();

    #define ACCESSORS(symbol)                                           \
      decltype(DOCTEST_CAT(_,symbol)) symbol () const noexcept {        \
        return DOCTEST_CAT(_,symbol);                                   \
      }                                                                 \
      options symbol (decltype(DOCTEST_CAT(_,symbol)) value) noexcept { \
        auto result = *this;                                            \
        result.DOCTEST_CAT(_,symbol) = value;                           \
        return result;                                                  \
      }
    ACCESSORS(junit)
    ACCESSORS(xml)
    ACCESSORS(success)
    ACCESSORS(no_intro)
    ACCESSORS(no_version)
    ACCESSORS(no_colors)
    ACCESSORS(no_path_filenames)
    ACCESSORS(no_skipped_summary)
    ACCESSORS(no_time_in_output)
    ACCESSORS(gnu_file_line)
    #undef ACCESSORS
  };

private:
  /** Executable that the test-case lives in */
  executable _exe;

  /** Test suite identifier */
  std::string _suite;

  /** Test case identifier */
  std::string _name;

  /** Subcase identifier */
  std::string _subcase;

public:
  /** Constructs a test-case from its triple */
  inline test_case(std::string suite, std::string name, std::string subcase)
    : _exe(executable::current()), _suite(suite), _name(name), _subcase(subcase)
  { }

  /** @return the currently running test case */
  static inline test_case current() noexcept {
    const auto &cs = *doctest::detail::g_cs;
    const auto &tc = *cs.currentTest;
    if (cs.subcaseStack.empty()) {
      return test_case(tc.m_test_suite, tc.m_name, "");
    } else {
      return test_case(tc.m_test_suite, tc.m_name, cs.subcaseStack.back().m_name.c_str());
    }
  }

  /** @return the internal version of the test case */
  inline test_case internal() noexcept {
    return test_case("INTERNAL", _name, _subcase);
  }

  /** @return the result of executing the test case with the specified options */
  inline subprocess::result run(options opts = { }) {
    auto args = std::vector<std::string> { };

    if (!_suite.empty())           args.push_back("-ts=" + _suite);
    if (!_name.empty())            args.push_back("-tc=" + _name);
    if (!_subcase.empty())         args.push_back("-sc=" + _subcase);
    if (opts.junit())              args.push_back("--reporters=junit");
    if (opts.xml())                args.push_back("--reporters=xml");
    if (opts.success())            args.push_back("--success");
    if (opts.no_intro())           args.push_back("--no-intro");
    if (opts.no_version())         args.push_back("--no-version");
    if (opts.no_colors())          args.push_back("--no-colors");
    if (opts.no_path_filenames())  args.push_back("--no-path-filenames");
    if (opts.no_skipped_summary()) args.push_back("--no-skipped-summary");
    if (opts.no_time_in_output())  args.push_back("--no-time-in-output");
    if (opts.gnu_file_line())      args.push_back("--gnu-file-line");

    return _exe.run(args);
  }
};

/** Required to be out-of-line to avoid errors */
test_case::options::options() = default;

} // namespace utils
