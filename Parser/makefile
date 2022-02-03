CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

parse: parser.c parser.h 
	$(CC) parser.c parser.h -o parser $(COMMON) $(SANITIZE) $(VALGRIND) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)

clean:
	rm -fr parser

