/**
 * @file utils/callback.h
 * @brief Provides a callback utility which tracks call history
 */

#pragma once

#include <functional>
#include <stdexcept>
#include <type_traits>

namespace utils {

/**
 * utils::callback<Fn> is analogous to std::function<Fn>,
 * except it automatically tracks call histories.
 * This is to support patterns in function where
 * we need to confirm that the callback was called,
 * without having to resort to hacks like shared state.
 *
 * It is also used to construct utils::spy
 */
template <typename Fn>
class callback;

template <typename R, typename... Args>
class callback<R(Args...)> {
public:
    using function_type = std::function<R(Args...)>;
    using args_type     = std::tuple<typename std::decay<Args>::type ...>;
    using return_type   = R;

private:
    /** @brief Ignores all arguments and returns a default-constructed R value */
    static R _default_handler(Args...) noexcept {
      return { };
    }

    /** Currently installed handler */
    function_type _handler;

    /** Historical arguments supplied to the callback */
    std::vector<args_type> _call_history { };

public:
    /** @brief Constructs a callback with the default handler */
    callback()
        : _handler(_default_handler)
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

template <typename... Args>
class callback<void(Args...)> {
public:
    using function_type = std::function<void(Args...)>;
    using args_type     = std::tuple<typename std::decay<Args>::type ...>;
    using return_type   = void;

private:
    /** @brief Ignores all arguments and returns a default-constructed R value */
    static return_type _default_handler(Args...) noexcept { }

    /** Currently installed handler */
    function_type _handler;

    /** Historical arguments supplied to the callback */
    std::vector<args_type> _call_history { };

public:
    /** @brief Constructs a callback with the default handler */
    callback()
        : _handler(_default_handler)
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
        _handler(std::forward<InnerArgs>(args)...);
    }

};

} // namespace utils
