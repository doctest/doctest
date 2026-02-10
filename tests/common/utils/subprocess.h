/**
 * @file utils/subprocess.h
 * @brief C++ convenience wrapper around sheredom/subprocess.h
 */

#pragma once

#include <doctest/parts/public/warnings.h>

#include <utils/text.h>

#include <stdexcept>
#include <array>
#include <string>
#include <vector>
#include <cerrno>

DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
DOCTEST_CLANG_SUPPRESS_WARNING("-Wgnu-anonymous-struct")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wnested-anon-types")
#include "thirdparty/subprocess.h"
DOCTEST_CLANG_SUPPRESS_WARNING_POP

namespace utils {
namespace subprocess {

#if defined(__cpp_exceptions)
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wweak-vtables")
class error : public std::runtime_error {
public:
  using runtime_error::runtime_error;
};
DOCTEST_CLANG_SUPPRESS_WARNING_POP
#endif // defined(__cpp_exceptions)

namespace detail {

/** -fno-exceptions friendly throw() method */
[[noreturn]] inline void fail(const char *reason) {
  static_cast<void>(reason);
#if defined(__cpp_exceptions)
  throw subprocess::error(reason);
#else
  std::abort();
#endif // defined(__cpp_exceptions)
}

/** @return the contents of @p{fp} */
inline std::string read(FILE *fp) {
  auto result = std::string { };
  auto block  = std::array<char, 256u> { };
  while (!std::feof(fp) && std::fgets(block.data(), static_cast<int>(block.size()), fp)) {
    result.append(block.data());
  }

  if (std::ferror(fp)) {
    detail::fail("failed to read");
  }

  return result;
}

} // namespace detail

enum class option : int {
  combined_stdout_stderr = subprocess_option_combined_stdout_stderr,
  inherit_environment    = subprocess_option_inherit_environment,
  enable_async           = subprocess_option_enable_async,
  no_window              = subprocess_option_no_window,
  search_user_path       = subprocess_option_search_user_path,
};

inline option operator|(option lhs, option rhs) noexcept {
  return static_cast<option>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4820)
struct result {
  int exit_code;
  std::string out;
  std::string err;

  inline bool ok() const noexcept { return exit_code == 0; }
};
DOCTEST_MSVC_SUPPRESS_WARNING_POP

static result run(std::string exe, std::vector<std::string> args, option options) {
  auto command_line = std::vector<const char *> { };
  command_line.push_back(exe.c_str());
  for (const auto &arg: args) { command_line.push_back(arg.c_str()); }
  command_line.push_back(nullptr);

  auto state = subprocess_s { };
  if (subprocess_create(command_line.data(), static_cast<int>(options), &state) != 0) {
    detail::fail("failed to spawn process");
  }

  auto exit_code = int { };
  if (subprocess_join(&state, &exit_code) != 0) {
    detail::fail("failed to join process");
  }

  return {
    exit_code,
    text::sanitize(detail::read(subprocess_stdout(&state))),
    text::sanitize(detail::read(subprocess_stderr(&state))),
  };
}

} // namespace subprocess
} // namespace utils
