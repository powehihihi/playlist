#include "playlist.hpp"
#include "UI.hpp"
#include "App.hpp"
#include <curses.h>
#include <list>
#include <memory>
#include <utility>
#include <iostream>

int main() {
  Song s1 = {"NewJeans - Ditto", 186}, s2 = {"blackbear - @ my worst", 191},
       s3 = {"347aidan - MEMORIES!", 138};
  App Myapp(std::list<Song>{s1, s2, s3}, -1);
  Myapp.Run();
  return 0;
}
