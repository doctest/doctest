#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

#ifdef DOCTEST_CONFIG_NO_MULTITHREADING
    template <typename T>
    using Atomic = T;
#else // DOCTEST_CONFIG_NO_MULTITHREADING
    template <typename T>
    using Atomic = std::atomic<T>;
#endif // DOCTEST_CONFIG_NO_MULTITHREADING

#if defined(DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS) || defined(DOCTEST_CONFIG_NO_MULTITHREADING)
    template <typename T>
    using MultiLaneAtomic = Atomic<T>;
#else // DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS
    // Provides a multilane implementation of an atomic variable that supports add, sub, load,
    // store. Instead of using a single atomic variable, this splits up into multiple ones,
    // each sitting on a separate cache line. The goal is to provide a speedup when most
    // operations are modifying. It achieves this with two properties:
    //
    // * Multiple atomics are used, so chance of congestion from the same atomic is reduced.
    // * Each atomic sits on a separate cache line, so false sharing is reduced.
    //
    // The disadvantage is that there is a small overhead due to the use of TLS, and load/store
    // is slower because all atomics have to be accessed.
    template <typename T>
    class MultiLaneAtomic
    {
        struct CacheLineAlignedAtomic
        {
            Atomic<T> atomic{};
            char padding[DOCTEST_MULTI_LANE_ATOMICS_CACHE_LINE_SIZE - sizeof(Atomic<T>)];
        };
        CacheLineAlignedAtomic m_atomics[DOCTEST_MULTI_LANE_ATOMICS_THREAD_LANES];

        static_assert(sizeof(CacheLineAlignedAtomic) == DOCTEST_MULTI_LANE_ATOMICS_CACHE_LINE_SIZE,
                      "guarantee one atomic takes exactly one cache line");

    public:
        T operator++() DOCTEST_NOEXCEPT { return fetch_add(1) + 1; }

        T operator++(int) DOCTEST_NOEXCEPT { return fetch_add(1); }

        T fetch_add(T arg, std::memory_order order = std::memory_order_seq_cst) DOCTEST_NOEXCEPT {
            return myAtomic().fetch_add(arg, order);
        }

        T fetch_sub(T arg, std::memory_order order = std::memory_order_seq_cst) DOCTEST_NOEXCEPT {
            return myAtomic().fetch_sub(arg, order);
        }

        operator T() const DOCTEST_NOEXCEPT { return load(); }

        T load(std::memory_order order = std::memory_order_seq_cst) const DOCTEST_NOEXCEPT {
            auto result = T();
            for(auto const& c : m_atomics) {
                result += c.atomic.load(order);
            }
            return result;
        }

        T operator=(T desired) DOCTEST_NOEXCEPT { // lgtm [cpp/assignment-does-not-return-this]
            store(desired);
            return desired;
        }

        void store(T desired, std::memory_order order = std::memory_order_seq_cst) DOCTEST_NOEXCEPT {
            // first value becomes desired", all others become 0.
            for(auto& c : m_atomics) {
                c.atomic.store(desired, order);
                desired = {};
            }
        }

    private:
        // Each thread has a different atomic that it operates on. If more than NumLanes threads
        // use this, some will use the same atomic. So performance will degrade a bit, but still
        // everything will work.
        //
        // The logic here is a bit tricky. The call should be as fast as possible, so that there
        // is minimal to no overhead in determining the correct atomic for the current thread.
        //
        // 1. A global static counter laneCounter counts continuously up.
        // 2. Each successive thread will use modulo operation of that counter so it gets an atomic
        //    assigned in a round-robin fashion.
        // 3. This tlsLaneIdx is stored in the thread local data, so it is directly available with
        //    little overhead.
        Atomic<T>& myAtomic() DOCTEST_NOEXCEPT {
            static Atomic<size_t> laneCounter;
            DOCTEST_THREAD_LOCAL size_t tlsLaneIdx =
                    laneCounter++ % DOCTEST_MULTI_LANE_ATOMICS_THREAD_LANES;

            return m_atomics[tlsLaneIdx].atomic;
        }
    };
#endif // DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS


} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
