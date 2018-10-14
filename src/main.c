#include <ncurses.h>

typedef struct points {
  int x;
  int y;
  struct points *next;
} Points;

typedef struct {
  Points *snake;
  Points *foods;
  int xmax;
  int ymax;
} Board;

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
