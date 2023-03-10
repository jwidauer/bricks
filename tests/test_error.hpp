#pragma once

#include <stdexcept>

namespace bricks::test {

/**
 * @brief Exception used during testing.
 */
class test_error : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

}  // namespace bricks::test
