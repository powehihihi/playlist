cmake_minimum_required(VERSION 3.0)
project(MusicPlayer)

add_compile_options(-Wall -Wextra -Wpedantic -g)

find_package(Catch2 3 REQUIRED)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(
  tests
  src/playlist.cpp
  src/SharedState.cpp
  test/test.cpp
  )
target_compile_definitions(tests PRIVATE TEST)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_executable(
  MusicPlayerUI
  src/playlist.cpp
  src/SharedState.cpp
  src/UI.cpp
  src/App.cpp
  src/main.cpp
  )

target_compile_definitions(MusicPlayerUI PRIVATE UIbuild)
target_link_libraries(MusicPlayerUI ${CURSES_LIBRARIES})
