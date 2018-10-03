#include <ncurses.h>

int main()
{
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  printw("Print statement\n");
  refresh();
  endwin();

  return 0;
}
