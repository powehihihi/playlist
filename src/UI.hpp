#ifndef UI_HPP
#define UI_HPP

#include <memory>
#include <ncurses.h>
#include <string>
#include "playlist.hpp"

class UI {
 private:
  //std::shared_ptr<SharedState> state;
  int h, w, xwin, ywin;
  WINDOW * frame;
 public:
  //UI(std::shared_ptr<SharedState> state);
  UI();
  ~UI();
  void Update(const Song s, int time);
  static void ShowKeys();
  static void AddNewSongWindow();
};
#endif // !UI_HPP
