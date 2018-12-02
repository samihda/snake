default: build

build:
	gcc -o snake src/main.c -Wall -lncurses
