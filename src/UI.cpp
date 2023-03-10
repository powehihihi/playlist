#include "UI.hpp"
#include <curses.h>

UI::UI() {
  initscr();			/* Start curses mode 		*/
  noecho();
  cbreak();			/* Line buffering disabled, Pass on evertything to me 		*/
  curs_set(0);
  //mvprintw(LINES-1, 0, "Hotkeys: \t<space>-Pause/Resume\t");
  refresh();

  h=4;
  w=COLS/4;
  xwin = (COLS-w)/2;
  ywin = (LINES-h)/2;

  //frame = newwin(h, w, ywin, xwin);
  frame = newwin(h, w, ywin, xwin);
  wrefresh(frame);
  box(frame, 0, 0);
  mvwaddch(frame, h-1, 2, ACS_RTEE);
  mvwaddch(frame, h-1, 8, ACS_LTEE);
  mvwaddch(frame, h-1, w-3, ACS_LTEE);
  mvwaddch(frame, h-1, w-9, ACS_RTEE);
}

UI::~UI() {
  endwin();
}

void UI::Update(const Song s, int time) {
  std::string emptyName (w-2, ' '), emptyTime(5, ' ');
  mvwprintw(frame, 1, 1, "%s", emptyName.c_str());
  mvwprintw(frame, 2, 1, "%s", emptyName.c_str());
  int x = (int)s.Name.size() > w-2 ? 1 : (w-s.Name.size())/2;
  mvwprintw(frame, 1, x, "%s", s.Name.c_str());
  mvwprintw(frame, h-1, w-8, "%s", emptyTime.c_str());
  mvwprintw(frame, h-1, w-8, "%02d:%02d", s.Duration/60, s.Duration%60);
  mvwprintw(frame, h-1, 3, "%s", emptyTime.c_str());
  mvwprintw(frame, h-1, 3, "%02d:%02d", time/60, time%60);
  wrefresh(frame);
}

