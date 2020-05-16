#pragma once
#include <functional>  // std::less
#include <vector>  // std::vector
#include <utility>  // std::swap

namespace bk {

// binary heap
// based on design of std::priority_queue
// https://en.cppreference.com/w/cpp/container/priority_queue

template <
  class T,
  class Container = std::vector<T>,
  class Compare = std::less<typename Container::value_type>
> class Heap {
 public:
  using container_type = Container;
  using value_compare = Compare;
  using value_type = typename Container::value_type;
  using size_type =  typename Container::size_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;

  Heap();
  explicit Heap(const Compare& compare);
  Heap(const Heap& other);
  Heap(Heap&& other);

  ~Heap() = default;

  // assignment
  Heap& operator=(const Heap& other) = default;
  Heap& operator=(Heap&& other) = default;

  // access
  [[nodiscard]] const_reference top() const;

  // capacity
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;

  // modifiers
  void push(const value_type& t);
  void push(value_type&& t);

  template <class... Args>
  void emplace(Args&&... args);

  void pop();

  void swap(Heap& other) noexcept;

 private:
  void heap_up(size_type i);
  void heap_down(size_type i);

  static inline size_type parent_idx(size_type i) {
    return (i - 1) / 2;
  }

  static inline size_type left_idx(size_type i) {
    return 2 * i + 1;
  }

  static inline size_type right_idx(size_type i) {
    return 2 * i + 2;
  }

  Compare compare_;
  Container container_;
};

template <class T, class Container, class Compare>
Heap<T, Container, Compare>::Heap(const Compare& compare)
  : compare_(compare)
  , container_() {}

template <class T, class Container, class Compare>
Heap<T, Container, Compare>::Heap()
  : Heap(Compare()) {}

template <class T, class Container, class Compare>
Heap<T, Container, Compare>::Heap(const Heap<T, Container, Compare>& other)
  : compare_(other.compare_)
  , container_(other.container_) {}

template <class T, class Container, class Compare>
Heap<T, Container, Compare>::Heap(Heap<T, Container, Compare>&& other)
  : compare_(std::move(other.compare_))
  , container_(std::move(other.container_)) {}

template <class T, class Container, class Compare>
[[nodiscard]]
typename Heap<T, Container, Compare>::const_reference
Heap<T, Container, Compare>::top() const {
  return container_.front();
}

template <class T, class Container, class Compare>
[[nodiscard]] bool Heap<T, Container, Compare>::empty() const noexcept {
  return container_.empty();
}

template <class T, class Container, class Compare>
[[nodiscard]]
typename Heap<T, Container, Compare>::size_type
Heap<T, Container, Compare>::size() const noexcept {
  return container_.size();
}

template <class T, class Container, class Compare>
void Heap<T, Container, Compare>::push(const value_type& t) {
  container_.emplace_back(t);
  heap_up(size() - 1);
}

template <class T, class Container, class Compare>
void Heap<T, Container, Compare>::push(value_type&& t) {
  container_.emplace_back(std::move(t));
  heap_up(size() - 1);
}



template <class T, class Container, class Compare>
void Heap<T, Container, Compare>::heap_up(size_type i) {
  while (i && compare_(container_[parent_idx(i)], container_[i])) {
    std::swap(container_[parent_idx(i)], container_[i]);
    i = parent_idx(i);
  }
}

template <class T, class Container, class Compare>
void Heap<T, Container, Compare>::heap_down(size_type i) {
  auto next = i;

  while (true) {
    const auto left = left_idx(next);
    const auto right = right_idx(next);

    auto largest = next;

    if (left < size() && !compare_(container_[left], container_[largest])) {
      largest = left;
    }

    if (right < size() && !compare_(container_[right], container_[largest])) {
      largest = right;
    }

    if (largest != next) {
      std::swap(container_[largest], container_[next]);
      next = largest;
    } else {
      break;
    }
  }
}

template <class T, class Container, class Compare>
template <class... Args>
void Heap<T, Container, Compare>::emplace(Args&&... args) {
  container_.emplace_back(std::forward<Args>(args)...);
  heap_up(size() - 1);
}

template <class T, class Container, class Compare>
void Heap<T, Container, Compare>::pop() {
  container_.front() = container_.back();
  container_.pop_back();
  heap_down(0);
}

template <class T, class Container, class Compare>
void Heap<T, Container, Compare>::swap(Heap<T, Container, Compare>& other) noexcept {
  (void)other;
  using std::swap;
  swap(container_, other.container_);
  swap(compare_, other.compare_);
}

} // namespace bk

namespace std {

template <class T, class Container, class Compare>
void swap(bk::Heap<T, Container, Compare>& lhs,
          bk::Heap<T, Container, Compare>& rhs) noexcept {
  lhs.swap(rhs);
}


} // namespace std
