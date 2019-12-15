CC = gcc

all:
	$(CC) -Wall -Wpedantic -Wextra --std=c99 src/main.c src/interface.c -o fulux
