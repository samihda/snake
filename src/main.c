#include "snake.h"
#include <ncurses.h>

enum direction getDirection(enum direction dir);
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

  seedRandomGen();

  dir = RIGHT;
  board = createBoard(createSnake(createPoints(3, 2),
                                  createPoints(2, 2)),
                      createRandomInt(col),
                      createRandomInt(row),
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
