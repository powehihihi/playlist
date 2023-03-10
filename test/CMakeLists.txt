cmake_minimum_required(VERSION 3.0)
project(MusicPlayer)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -g)

find_package(Catch2 3 REQUIRED)

add_executable(
  tests
  ../src/playlist.cpp
  ../src/SharedState.cpp
  test.cpp
  )
target_compile_definitions(tests PRIVATE TEST)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

