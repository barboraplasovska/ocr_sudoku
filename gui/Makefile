# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl SDL_image` -Wall -Wextra -Werror -O1
#CDLIBS = -L/opt/homebrew/lib -lSDLmain -lSDL #-Wl,-framework,Cocoa
LDLIBS = `pkg-config --libs gtk+-3.0 sdl SDL_image` -lm

gui: gui.c image_processing.h
	$(CC) $(CFLAGS) -o gui gui.c image_processing.c $(LDLIBS)

image_processing.o: image_processing.c image_processing.h
	$(CC) $(CFLAGS) -c image_processing.c image_processing.h $(LDLIBS)

# remove object files and executable when user executes "make clean"
clean:
	${RM} -f *.o
	${RM} -f *.gch
	${RM} gui
# END