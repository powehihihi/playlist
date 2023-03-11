#include "UI.hpp"
#include <curses.h>
#include <ncurses.h>

UI::UI() {
  initscr();			/* Start curses mode 		*/
  noecho();
  cbreak();			/* Line buffering disabled, Pass on evertything to me 		*/
  curs_set(0);
  keypad(stdscr, TRUE);
  refresh();

  h=4;
  w=COLS/4;
  xwin = (COLS-w)/2;
  ywin = (LINES-h)/2;

  Box = newwin(h, w, ywin, xwin);
  frame = newwin(h-2, w-2, ywin+1, xwin+1);
  wrefresh(Box);
  box(Box, 0, 0);
  mvwaddch(Box, h-1, 2, ACS_RTEE);
  mvwaddch(Box, h-1, 8, ACS_LTEE);
  mvwaddch(Box, h-1, w-3, ACS_LTEE);
  mvwaddch(Box, h-1, w-9, ACS_RTEE);
}

UI::~UI() {
  endwin();
}

void UI::Update(const Song s, int time) {
  std::string emptyName (w-2, ' '), emptyTime(5, ' ');
  wclear(frame);
  int x = (int)s.Name.size() > w-2 ? 1 : (w-s.Name.size())/2;
  mvwprintw(frame, 0, x, "%s", s.Name.c_str());
  mvwprintw(frame, h-1, w-8, "%s", emptyTime.c_str());
  mvwprintw(Box, h-1, w-8, "%02d:%02d", s.Duration/60, s.Duration%60);
  mvwprintw(Box, h-1, 3, "%s", emptyTime.c_str());
  mvwprintw(Box, h-1, 3, "%02d:%02d", time/60, time%60);
  wrefresh(Box);
  wrefresh(frame);
}

