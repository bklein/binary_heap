cmake_minimum_required(VERSION 3.17)

project(heap_cpp
  VERSION 0.0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Weffc++ -Wpedantic")

add_library(heap_cpp INTERFACE)
target_include_directories(heap_cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(heap_cpp INTERFACE cxx_std_17)
target_compile_options(heap_cpp INTERFACE -Werror -Wall -Wextra)


add_executable(heap_demo demo.cc)
target_link_libraries(heap_demo PRIVATE heap_cpp)

add_executable(heap_tests heap_tests.cc)
target_link_libraries(heap_tests PRIVATE heap_cpp)
