default: build

build:
	gcc -o snake src/main.c src/snake.c -Wall -lncurses
