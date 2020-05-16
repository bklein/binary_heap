#include <deque>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "heap.hpp"
#include "random_uniform_generator.hpp"

namespace {

using namespace bk;

template <typename T>
using DescHeap = Heap<T, std::vector<T>, std::less<T>>;

template <typename T>
using AscHeap = Heap<T, std::vector<T>, std::greater<T>>;

TEST_CASE("Default construct") {
  DescHeap<int> heap;
  REQUIRE(heap.size() == 0);
  REQUIRE(heap.empty());
}

TEST_CASE("Custom comparator") {
  auto comp =
    [] (int a, int b) -> bool {
      return a > b;
    };
  using heap_t = Heap<int, std::vector<int>, decltype(comp)>;
  heap_t heap(comp);
  for (int i=0; i<3; ++i)
    heap.push(i);

  REQUIRE(heap.size() == 3);
  REQUIRE(heap.top() == 0);
}

TEST_CASE("Alternative container") {
  using heap_t = Heap<int, std::deque<int>>;
  heap_t heap;

  for (int i=0; i<5; ++i)
    heap.push(i);

  for (int i=4; i>=0; --i) {
    REQUIRE(heap.top() == i);
    heap.pop();
  }
  REQUIRE(heap.empty());
}

TEST_CASE("Copy construct") {
  DescHeap<int> a;

  for (int i=0; i<5; ++i) {
    a.push(i);
  }
  REQUIRE(a.size() == 5);
  DescHeap<int> b(a);
  REQUIRE(a.size() == 5);
  REQUIRE(b.size() == 5);
  for (int i=0; i<5; ++i) {
    REQUIRE(a.top() == b.top());
    a.pop();
    b.pop();
  }
}

TEST_CASE("Move construct") {
  AscHeap<int> a;

  for (int i=0; i<5; ++i) {
    a.push(i);
  }
  REQUIRE(a.size() == 5);
  AscHeap<int> b(std::move(a));
  REQUIRE(a.size() == 0);
  REQUIRE(b.size() == 5);
  for (int i=0; i<5; ++i) {
    REQUIRE(b.top() == i);
    b.pop();
  }
}

TEST_CASE("Copy asignment") {
  DescHeap<int> a;
  DescHeap<int> b;

  for (int i=0; i<5; ++i) {
    a.push(i);
  }
  REQUIRE(a.size() == 5);
  REQUIRE(b.size() == 0);
  b = a;
  REQUIRE(a.size() == 5);
  REQUIRE(b.size() == 5);
  for (int i=0; i<5; ++i) {
    REQUIRE(a.top() == b.top());
    a.pop();
    b.pop();
  }
}

TEST_CASE("Move assignment") {
  AscHeap<int> a;
  AscHeap<int> b;

  for (int i=0; i<5; ++i) {
    a.push(i);
  }
  REQUIRE(a.size() == 5);
  REQUIRE(b.size() == 0);
  b = std::move(a);
  REQUIRE(a.size() == 0);
  REQUIRE(b.size() == 5);
  for (int i=0; i<5; ++i) {
    REQUIRE(b.top() == i);
    b.pop();
  }
}

TEST_CASE("Sorted descending") {
  RandomUniformGenerator rand;

  const auto N = std::numeric_limits<uint16_t>::max();
  DescHeap<int> heap;
  for (int i=0; i<N; ++i)
    heap.push(rand.next());

  REQUIRE(heap.size() == N);

  auto last = heap.top();
  heap.pop();
  while (!heap.empty()) {
    REQUIRE(last >= heap.top());
    last = heap.top();
    heap.pop();
  }
  REQUIRE(heap.empty());
}

TEST_CASE("Sorted ascending") {
  RandomUniformGenerator rand;
  const auto N = std::numeric_limits<uint16_t>::max();
  AscHeap<int> heap;
  for (int i=0; i<N; ++i)
    heap.push(rand.next());

  REQUIRE(heap.size() == N);

  auto last = heap.top();
  heap.pop();
  while (!heap.empty()) {
    REQUIRE(last <= heap.top());
    last = heap.top();
    heap.pop();
  }
  REQUIRE(heap.empty());
}

TEST_CASE("empty") {
  DescHeap<int> heap;
  REQUIRE(heap.empty());
  heap.push(1);
  REQUIRE(!heap.empty());
}

TEST_CASE("size") {
  AscHeap<int> heap;
  for (int i=0; i<3; ++i) {
    REQUIRE(heap.size() == i);
    heap.push(i);
    REQUIRE(heap.size() == (i + 1));
  }
}

TEST_CASE("push ref") {
  AscHeap<std::string> heap;
  const std::string s("a string");
  heap.push(s);
  REQUIRE(heap.size() == 1);
}

TEST_CASE("push move") {
  AscHeap<std::string> heap;
  const std::string s("a string");
  heap.push(std::move(s));
  REQUIRE(heap.size() == 1);
}

struct Foo {
  Foo(int x, double y)
    : x_(x)
    , y_(y) {}

  int x_;
  double y_;
};

bool operator>(const Foo& a, const Foo& b) {
  return a.x_ > b.x_;
}

bool operator==(const Foo& a, const Foo& b) {
  return a.x_ == b.x_;
}

TEST_CASE("emplace") {
  AscHeap<Foo> heap;
  heap.emplace(42, 3.14);
  REQUIRE(heap.size() == 1);
  const auto& f = heap.top();
  REQUIRE(f == Foo(42, 3.14));
}

TEST_CASE("swap") {
  AscHeap<int> a;
  AscHeap<int> b;

  for (int i=0; i<3; ++i)
    a.push(i);

  std::swap(a, b);

  REQUIRE(a.size() == 0);
  REQUIRE(b.size() == 3);

  for (int i=0; i<3; ++i) {
    REQUIRE(b.top() == i);
    b.pop();
  }
  REQUIRE(b.empty());
}

} // namespace anonymous
