CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 `pkg-config --cflags gtk+-3.0 sdl SDL_image`
LDLIBS = `pkg-config --libs gtk+-3.0 sdl SDL_image` -lm

gui: digitRecog.o network.o solver.o image_processing.o
	$(CC) $(CFLAGS) -o gui -Iutil gui.c digitRecog.o network.o solver.o image_processing.o $(LDLIBS)

image_processing.o: image_processing.c image_processing.h
	$(CC) $(CFLAGS) -c image_processing.c image_processing.h $(LDLIBS)

digitRecog.o: digitRecog.c digitRecog.h
	$(CC) $(CFLAGS) -c digitRecog.c digitRecog.h

network.o: network.c network.h
	$(CC) $(CFLAGS) -c network.c network.h

solver.o: solver.c solver.h
	$(CC) $(CFLAGS) -c solver.c solver.h

clean:
	${RM} -f *.o
	${RM} -f *.gch
	${RM} -f *.out
	${RM} -rf *.dSYM
	${RM} -rf *.bmp
	${RM} gui