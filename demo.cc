#include <cassert>
#include <cstdlib>

#include <functional>
#include <iostream>

#include "heap.hpp"
#include "random_uniform_generator.hpp"

using namespace bk;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  Heap<int, std::vector<int>, std::greater<int>> heap;

  const auto N = std::numeric_limits<uint16_t>::max();

  RandomUniformGenerator rand;
  for (int i=0; i<N; ++i)
    heap.push(rand.next());

  assert(heap.size() == N);
  std::cout << "verifying" << std::endl;

  auto last = heap.top();
  heap.pop();
  while (!heap.empty()) {
    assert(std::less_equal<int>{}(last, heap.top()));
    last = heap.top();
    std::cout << heap.top() << std::endl;
    heap.pop();
  }
  std::cout << "ok" << std::endl;

  return EXIT_SUCCESS;
}
