#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O2 -g -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=
USERFILES = display.o maze.o

all:  game

game: ${USERFILES}
	${CC} ${CFLAGS} game.c ${USERFILES}  -o game ${LDFLAGS}

clean:
	-rm -f  game *.o

install:


.PHONY: all install clean

# end of development makefile
