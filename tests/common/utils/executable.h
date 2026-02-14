/**
 * @file utils/executable.h
 * @brief High-level manipulation and running of executables
 */

#pragma once

#include <doctest/parts/private/context_state.h>
#include <utils/subprocess.h>

#include <string>
#include <vector>

namespace utils {

class executable {
private:
  std::string _exe;

  inline executable(std::string exe) noexcept
    : _exe(exe)
  { }

public:
  /** @return the currentely running executable */
  static executable current() noexcept {
    return { doctest::detail::g_cs->binary_name.c_str() };
  }

  /**
   * Runs the executable with the given args,
   * and either produces a run_result or raises an error
   */
  subprocess::result run(std::vector<std::string> args) const {
    const auto options = subprocess::option::inherit_environment
                       | subprocess::option::no_window
                       | subprocess::option::search_user_path;
    return subprocess::run(_exe, args, options);
  }
};

} // namespace utils
