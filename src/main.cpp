#include "playlist.hpp"
#include "UI.hpp"
#include "App.hpp"
#include <curses.h>
#include <list>
#include <memory>
#include <utility>
#include <iostream>

int main() {
  Song s1 = {"SHTRIHCOD - ARLONG PARK", 114}, s2 = {"pfctlord - #3", 226};
  App Myapp(std::list<Song>{s1, s2}, -1);
  Myapp.Run();
  return 0;
}
