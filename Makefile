CC=clang
CFLAGS=-Wall -Wextra -fsanitize=address -g -Wpedantic
LFLAGS=-lX11 -lGL
game: ./example.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
