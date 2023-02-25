#pragma once

#include <chrono>
#include <future>
#include <thread>

namespace bricks {

/**
 * @brief A timer that can be started and aborted.
 *
 * @details
 * The timer can be started with a duration. If the timer is not aborted, the completion token
 * returned by `start` will be ready after the duration has passed. If the timer is aborted, the
 * completion token will be ready immediately. Will abort all outstanding timers when it is
 * destroyed.
 *
 * Example:
 * @snippet timer_test.cpp timer-example
 */
class timer {
 public:
  /** @brief Default constructor. */
  timer() noexcept {};  // NOLINT (can't use default, since std::promise does not have a noexcept
                        // ctor)

  /** @brief A timer cannot be copied. */
  timer(const timer&) = delete;
  /** @brief A timer cannot be copied. */
  auto operator=(const timer&) -> timer& = delete;

  /** @brief Move constructor. */
  timer(timer&&) = default;
  /** @brief Move assignment operator. */
  auto operator=(timer&&) -> timer& = default;

  /**
   * @brief Destroy the timer object.
   *
   * @details
   * Will abort all outstanding timers.
   */
  ~timer() { abort(); };

  /**
   * @brief A completion token that can be used to wait for the timer to complete.
   *
   * @details
   * The completion token can be used to wait for the timer to complete. If the timer is not
   * aborted, the completion token will be ready after the duration has passed. If the timer is
   * aborted, the completion token will be ready immediately.
   *
   * Example:
   * @snippet timer_test.cpp timer-example
   */
  using completion_token = std::future<void>;

  /**
   * @brief Starts the timer, returning a completion token that will be completed when the timer
   * expires.
   *
   * Example:
   * @snippet timer_test.cpp timer-example
   *
   * @param duration The duration to wait before completing the token.
   *
   * @return A timer::completion_token that will be completed when the timer expires.
   */
  template <typename Rep = int64_t, typename Period = std::ratio<1>>
  [[nodiscard]] auto start(const std::chrono::duration<Rep, Period>& duration =
                               std::chrono::duration<Rep, Period>{}) const noexcept
      -> completion_token
  {
    return std::async(std::launch::async, [abortion_future = abortion_future_, duration]() {
      abortion_future.wait_for(duration);
    });
  }

  /**
   * @brief Aborts the timer.
   *
   * This function aborts the timer, causing any completion tokens returned by `start` to complete.
   *
   * Example:
   * @snippet timer_test.cpp timer-abort-example
   */
  inline auto abort() -> void
  {
    abortion_promise_.set_value();
    abortion_promise_ = {};
    abortion_future_ = abortion_promise_.get_future();
  }

 private:
  std::promise<void> abortion_promise_{};
  std::shared_future<void> abortion_future_{abortion_promise_.get_future()};
};

}  // namespace bricks
