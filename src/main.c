#include <ncurses.h>
#include <stdlib.h>

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

Points *createPoints(int x, int y);
Points *createSnake(Points *head, Points *tail);
Board *createBoard(Points *snake, Points *foods, int row, int col);

void renderSnake(Points *snake);

int main()
{
  initscr();

  /* `raw` / `cbreak` disable line buffering */
  /* `cbreak` captures control characters */
  cbreak();

  keypad(stdscr, TRUE);
  noecho();

  /* set cursor visibility */
  curs_set(0);

  /* TODO: check out `nodelay` */
  timeout(100);

  int row;
  int col;
  char ch;
  Board *board;

  getmaxyx(stdscr, row, col);

  board = createBoard(createSnake(createPoints(2, 3), createPoints(2, 2)), NULL, row, col);

  while ((ch = getch()) != 'q') {
    clear();

    renderSnake(board->snake);

    refresh();
  }

  endwin();

  return 0;
}

Points *createPoints(int x, int y)
{
  Points *p = malloc(sizeof(Points));
  p->x = x;
  p->y = y;
  p->next = NULL;

  return p;
}

Points *createSnake(Points *head, Points *tail)
{
  head->next = tail;
  return head;
}

Board *createBoard(Points *snake, Points *foods, int row, int col)
{
  Board *board = malloc(sizeof(*board));
  board->foods = foods;
  board->snake = snake;
  board->xmax = row;
  board->ymax = col;

  return board;
}

void renderSnake(Points *snake)
{
  while (snake) {
    mvaddch(snake->y, snake->x, ACS_BLOCK);
    snake = snake->next;
  }
}
