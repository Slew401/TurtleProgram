mCC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

extension: extension.c extension.h  
	$(CC) extension.c extension.h -o extension $(COMMON) $(SANITIZE) $(VALGRIND) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)

clean:
	rm -fr extension