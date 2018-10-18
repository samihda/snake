#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

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

enum direction { UP, DOWN, LEFT, RIGHT };

Points *createPoints(int x, int y);
Points *createSnake(Points *head, Points *tail);
Board *createBoard(Points *snake, Points *foods, int row, int col);
Points *moveSnake(Points *snake);
enum direction getDirection(enum direction dir, int c);
bool collided(Board *board);

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

  while (!collided(board) && (ch = getch()) != 'q') {
    clear();

    renderSnake(board->snake);

    refresh();

    board->snake = moveSnake(board->snake);
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
  board->xmax = col;
  board->ymax = row;

  return board;
}

Points *moveSnake(Points *snake)
{
  Points *head;

  free(snake->next);
  snake->next = NULL;

  head = createPoints(snake->x, snake->y + 1); // go downwards
  head->next = snake;

  snake = head;

  return snake;
}

bool collided(Board *board)
{
  if (board->snake->x == 0 || board->snake->y == 0) {
    return true;
  } else if (board->snake->x == board->xmax || board->snake->y == board->ymax) {
    return true;
  } else {
    return false;
  }
}

enum direction getDirection(enum direction dir, int c)
{
  switch (c) {
  case KEY_UP:
    if (dir != DOWN) return UP;

  case KEY_DOWN:
    if (dir != UP) return DOWN;

  case KEY_LEFT:
    if (dir != RIGHT) return LEFT;

  case KEY_RIGHT:
    if (dir != LEFT) return RIGHT;

  default:
    return dir;
  }
}

void renderSnake(Points *snake)
{
  while (snake) {
    mvaddch(snake->y, snake->x, ACS_BLOCK);
    snake = snake->next;
  }
}
