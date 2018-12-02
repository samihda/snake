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
  int foodX;
  int foodY;
  int xmax;
  int ymax;
} Board;

enum direction { UP, DOWN, LEFT, RIGHT };

Points *createPoints(int x, int y);
Points *createSnake(Points *head, Points *tail);
Board *createBoard(Points *snake, int foodX, int foodY, int row, int col);

void moveSnake(enum direction dir, Board *board);
enum direction getDirection(enum direction dir);

bool collided(Board *board);
bool suicide(Points *snake);

void dropLast(Points *list);
void render(Board *board);

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
  board = createBoard(createSnake(createPoints(3, 2),
                                  createPoints(2, 2)),
                      0,
                      0,
                      row,
                      col);

  while (!collided(board) && !suicide(board->snake)) {
    clear();

    render(board);

    refresh();

    dir = getDirection(dir);
    moveSnake(dir, board);
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

Board *createBoard(Points *snake, int foodX, int foodY, int row, int col)
{
  Board *board = malloc(sizeof(*board));
  board->snake = snake;
  board->foodX = foodX;
  board->foodY = foodY;
  board->xmax = col;
  board->ymax = row;

  return board;
}

void moveSnake(enum direction dir, Board *board)
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

  head = createPoints(board->snake->x + offsetx, board->snake->y + offsety);

  if (head->x == board->foodX && head->y == board->foodY) {
    board->foodX = 0;
    board->foodY = 0;
  } else {
    dropLast(board->snake);
  }

  head->next = board->snake;
  board->snake = head;
}

void dropLast(Points *list)
{
  while (list->next) {
    if (list->next->next) {
      list = list->next;
    } else {
      break;
    }
  }

  free(list->next);
  list->next = NULL;
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

void render(Board *board)
{
  Points *snake = board->snake;

  while (snake) {
    mvaddch(snake->y, snake->x, ACS_BLOCK);
    snake = snake->next;
  }

  if (board->foodX > 0 && board->foodY > 0) {
    mvaddch(board->foodY, board->foodX, ACS_DIAMOND);
  }
}
