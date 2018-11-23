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

Points *moveSnake(enum direction dir, Points *snake);
enum direction getDirection(enum direction dir);

bool collided(Board *board);
bool suicide(Points *snake);

void dropOne(Points *snake);
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
  Board *board;
  enum direction dir;

  getmaxyx(stdscr, row, col);

  dir = RIGHT;
  board = createBoard(createSnake(createPoints(3, 2), createPoints(2, 2)), NULL, row, col);

  while (!collided(board) && !suicide(board->snake)) {
    clear();

    renderSnake(board->snake);

    refresh();

    dir = getDirection(dir);
    board->snake = moveSnake(dir, board->snake);
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

Points *moveSnake(enum direction dir, Points *snake)
{
  Points *head;

  int offsetx = 0;
  int offsety = 0;

  switch (dir) {
  case UP:
    offsety = -1;
    break;
  case DOWN:
    offsety = 1;
    break;
  case LEFT:
    offsetx = -1;
    break;
  case RIGHT:
    offsetx = 1;
    break;
  }

  head = createPoints(snake->x + offsetx, snake->y + offsety);

  dropOne(snake);
  head->next = snake;

  snake = head;

  return snake;
}

void dropOne(Points *snake)
{
  while (snake->next) {
    if (snake->next->next) {
      snake = snake->next;
    } else {
      break;
    }
  }

  free(snake->next);
  snake->next = NULL;
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

bool suicide(Points *snake)
{
  Points *tail = snake->next;

  while (tail) {
    if (tail->x == snake->x && tail->y == snake->y) {
      return true;
    }

    tail = tail->next;
  }

  return false;
}

enum direction getDirection(enum direction dir)
{
  int c = getch();

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
