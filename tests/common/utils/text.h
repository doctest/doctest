/**
 * @file utils/text.h
 * @brief Provides general-purpose text utilities
 */

#pragma once

#include <doctest/parts/public/warnings.h>
#include <cctype>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace utils {
namespace text {

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

/**
 * Splits the input @p{text} by the specified @p{delimiter}.
 * If @p{keep_ends} is set, then the @p{delimiter} is kept
 * in the output strings, otherwise it is removed.
 * If consecutive @p{delimiter} values are found,
 * a blank string (or just the @p{delimiter}) will be
 * present in the result.
 */
inline std::vector<std::string> split(const std::string &text, char delimiter = '\n', bool keep_ends = true) noexcept {
    auto result = std::vector<std::string> { };
    auto start  = std::size_t { 0 };
    auto end    = text.find(delimiter);

    while (end != std::string::npos) {
        auto length = keep_ends? (end - start + 1) : (end - start);
        result.push_back(text.substr(start, length));

        start = end + 1;
        end = text.find(delimiter, start);
    }

    if (start < text.length()) {
        result.push_back(text.substr(start));
    }

    return result;
}

/**
 * Dedents the input @p{text} by the most common amount of leading whitespace.
 * Leading/trailing blank lines will also be removed
 * @see Python textwrap.dedent
 */
inline std::string dedent(const std::string &text) noexcept {
    const auto count_char = [](std::string line, char c) {
        size_t count = 0;
        while ((count < line.size()) && (line[count] == c)) { count++; }
        return count;
    };

    const auto is_blank = [](std::string line) {
        return std::all_of(line.begin(), line.end(), [](char c) { return std::isspace(c); });
    };

    // To support multiline raw-strings, we need to skip the first/last blank line
    auto lines = split(text);

    if ((lines.size() > 0) && is_blank(lines.front())) {
        lines.erase(lines.begin());
    }

    if ((lines.size() > 0) && is_blank(lines.back())) {
        lines.erase(lines.end() - 1);
    }

    // Past this point, we need to determine the max. common whitespace, and strip it
    auto prefix = std::numeric_limits<size_t>::max();
    for (const auto &line: lines) {
        prefix = std::min(prefix, count_char(line, ' '));
    }

    auto result = std::string();
    for (const auto &line: lines) {
        result += line.substr(prefix);
    }
    return result;
}

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

} // namespace text
} // namespace utils
