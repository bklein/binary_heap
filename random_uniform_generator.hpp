#pragma once
#include <limits>
#include <random>

namespace bk {

template <class T = int>
class RandomUniformGenerator {
 public:
  static_assert(std::numeric_limits<T>::is_integer);
  RandomUniformGenerator()
    : rd_()
    , gen_(rd_())
    , dist_(
        std::numeric_limits<T>::min(),
        std::numeric_limits<T>::max())
  {}

  T next() {
    return dist_(gen_);
  }

 private:
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<T> dist_;
};


}  // namespace bk
