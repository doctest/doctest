/**
 * @file utils/callback.h
 * @brief Provides a callback utility which tracks call history
 */

#pragma once

#include <doctest/parts/public/platform.h>
#include <doctest/parts/public/warnings.h>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace utils {

namespace detail {

/**
 * @brief typedefs and constants for for utils::callback<Fn>
 * We mostly need this to deal with some Win32 business
 */
template<typename Fn>
class callback_traits;

template <typename R, typename... Args>
class callback_traits<R(Args...)> {
public:
    using function_type = std::function<R(Args...)>;
    using args_type     = std::tuple<typename std::decay<Args>::type ...>;
    using return_type   = R;

    /** @brief Ignores all arguments and returns a default-constructed R value */
    static return_type default_handler(Args...) noexcept {
      return { };
    }
};

#if defined(DOCTEST_PLATFORM_WINDOWS) && (defined(_M_IX86) || defined(__i386__))
/** This is required to get around Win32 callconv annotations */
template <typename R, typename... Args>
class callback_traits<R __stdcall(Args...)> : public callback_traits<R(Args...)> { };
#endif // defined(DOCTEST_PLATFORM_WINDOWS)

} // namespace detail

/**
 * utils::callback<Fn> is analogous to std::function<Fn>,
 * except it automatically tracks call histories.
 * This is to support patterns in function where
 * we need to confirm that the callback was called,
 * without having to resort to hacks like shared state.
 */
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4820)
template <typename Fn>
class callback {
public:
    using traits_type   = detail::callback_traits<Fn>;
    using function_type = typename traits_type::function_type;
    using args_type     = typename traits_type::args_type;
    using return_type   = typename traits_type::return_type;

private:
    /** Currently installed handler */
    function_type _handler;

    /** Historical arguments supplied to the callback */
    std::vector<args_type> _call_history { };

public:
    /** @brief Constructs a callback with the default handler */
    callback()
        : _handler(traits_type::default_handler)
    { }

    /** @brief Constructs a callback with the supplied handler */
    callback(function_type fn)
        : _handler(fn)
    { }

    /** @brief Sets a new handler for the callback */
    callback &operator=(function_type fn) {
      set_handler(fn);
      return *this;
    }

    /** @brief Sets a new handler for the callback */
    void set_handler(function_type fn) {
      _handler = fn;
    }

    /** @return the call history of the callback  */
    const std::vector<args_type> &call_history() noexcept {
      return _call_history;
    }

    /** @return the most recent call history of the callback */
    const args_type &latest_call() {
      return _call_history.back();
    }

    /** @return the number of calls made to the callback */
    std::size_t call_count() noexcept {
      return _call_history.size();
    }

    /** @brief Records the arguments, and forwards them to the handler */
    template <typename... InnerArgs>
    return_type operator()(InnerArgs&&... args) {
        _call_history.push_back(args_type { args... });
        return _handler(std::forward<InnerArgs>(args)...);
    }
};
DOCTEST_MSVC_SUPPRESS_WARNING_POP

} // namespace utils
