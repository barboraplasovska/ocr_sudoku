CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1
LDLIBS = `pkg-config --cflags --libs gtk+-3.0 sdl SDL_image`

gui: gui.c
	$(CC) $(CFLAGS) -o gui gui.c digitRecog.c network.c image_processing.c $(LDLIBS) -lm -Iutil

image_processing.o: image_processing.c
	$(CC) $(CFLAGS) -c image_processing.c $(LDLIBS)

digitRecog.o: digitRecog.c network.o
	$(CC) $(CFLAGS) -c digitRecog.c network.o

network.o: network.c
	$(CC) $(CFLAGS) -c network.c

#solver.o: solver.c
#	$(CC) $(CFLAGS) -c solver.c

clean:
	${RM} -f *.o
	${RM} -f *.gch
	${RM} -f *.out
	${RM} -rf *.dSYM
	${RM} -rf *.bmp
	${RM} gui
	${RM} -f *.result
	${RM} grid*
	${RM} test
	${RM} unsolvedGrid
