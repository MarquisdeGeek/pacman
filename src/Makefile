ifndef PKG_CONFIG
PKG_CONFIG=pkg-config
endif

#CFLAGS = -Wall -D_DEBUG -g
CFLAGS = -Wall -Werror -pedantic
LDLIBS=$(shell $(PKG_CONFIG) --libs ncurses)

pacman: pacman.o player.o ghosts.o pills.o render.o board.o gameloop.o 
pacman.o: pacman.h render.h gameloop.h
player.o: pacman.h player.h ghosts.h pills.h board.h
ghosts.o: pacman.h ghosts.h board.h
board.o:  pacman.h board.h player.h ghosts.h pills.h
gameloop.o: pacman.h gameloop.h player.h ghosts.h pills.h board.h render.h
render.o: pacman.h render.h board.h
pills.o: pacman.h pills.h board.h

install:
	cat *.c *.h >pactext

clean:
	-rm -f *.o pacman pactext

# DO NOT DELETE
