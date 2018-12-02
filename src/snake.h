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

void seedRandomGen();
int createRandomInt(int max);

Points *createPoints(int x, int y);
Points *createSnake(Points *head, Points *tail);
Board *createBoard(Points *snake, int foodX, int foodY, int row, int col);

void moveSnake(enum direction dir, Board *board);

bool collided(Board *board);
bool suicide(Points *snake);

void dropLast(Points *list);
