#ifndef UI_HPP
#define UI_HPP

#include <curses.h>
#include <memory>
#include <ncurses.h>
#include <stdexcept>
#include <string>
#include <exception>
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


  static void ShowKeys() {
    std::string keys = "\t<N> New song\t\t<h> Prev song\t\t<Space> Pause\t\t<Anything> Play\t\t<l> Next song";
    mvprintw(LINES-2, 0, "%s", keys.c_str());
    refresh();
  }
  static Song AddNewSongWindow() {
    WINDOW * addwin = newwin(4, COLS/4, 1,1);
    WINDOW * inner = newwin(2, COLS/4-2, 1+1, 1+1);
    box(addwin, 0, 0);
    mvwprintw(addwin, 0, 1, "Song name: ");
    wrefresh(addwin);
    wrefresh(inner);
    int key;
    std::string name = "";
    for ( ; ; ) {
      key = getch();
      if (key=='\n')
        break;
      /*
      if (key==KEY_BREAK) {
        name = "";
        break;
      }
      */
      if (key==KEY_BACKSPACE) {
        if (!name.empty()) {
          name.erase(name.length()-1, 1);
          wclear(inner);
          wprintw(inner, "%s", name.c_str());
        }
      }
      else {
        name.push_back(static_cast<char>(key));
        wprintw(inner, "%c", name.back());
      }
      wrefresh(inner);
    }
    if (name.empty())
      throw std::runtime_error("Song with an empty name");
    std::string time;
    box(addwin, 0, 0);
    mvwprintw(addwin, 0, 1, "Song duration: ");
    wclear(inner);
    wrefresh(addwin);
    wrefresh(inner);
    for ( ; ; ) {
      key = getch();
      if (key=='\n')
        break;
      /*
      if (key==KEY_OPEN) {
        time="";
        break;
      }*/
      if (key==KEY_BACKSPACE) {
        if (!time.empty()) {
          time.erase(time.length()-1, 1);
          wclear(inner);
          wprintw(inner, "%s", time.c_str());
        }
      }
      else if (key>='0' && key<='9') {
        time.push_back(static_cast<char>(key));
        wprintw(inner, "%c", time.back());
      }
      wrefresh(inner);
    }
    wclear(inner);
    wclear(addwin);
    wborder(addwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(inner);
    wrefresh(addwin);
    delwin(inner);
    delwin(addwin);
    return Song(name, std::stoi(time));
  }
};

#endif // !UI_HPP
