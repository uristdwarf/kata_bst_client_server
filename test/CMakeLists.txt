cmake_minimum_required(VERSION 3.14)
enable_testing()

add_executable(
  tests
  client_test.cpp
  lib_test.cpp
  server_test.cpp
)
target_link_libraries(
  tests
  GTest::gtest_main
  client_lib
  server_lib
  lib
)

# target_link_libraries(test gtest gtest_main client_lib)

include(GoogleTest)
gtest_discover_tests(tests)
