#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void seedRandomGen()
{
  srand(time(0));
}

int createRandomInt(int max)
{
  return rand() % (max - 1) + 1;
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
    board->foodX = createRandomInt(board->xmax);
    board->foodY = createRandomInt(board->ymax);
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
