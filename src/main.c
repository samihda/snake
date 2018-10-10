#include <ncurses.h>

struct points {
  int x;
  int y;
  struct points *next;
};

typedef struct points Points;

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
