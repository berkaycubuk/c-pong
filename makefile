CC = gcc
CFLAGS = -lSDL2 -I$(LDIR)

LDIR = ./lib/
SRCDIR = ./src/

SOURCES = $(SRCDIR)*.c

all: build clean

build:
	$(CC) $(SOURCES) $(CFLAGS) -o pong

run:
	./pong

clean:
	rm pong
