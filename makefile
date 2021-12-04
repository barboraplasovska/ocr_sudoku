CC = gcc -fsanitize=address
CFLAGS = -Wall -Wextra -Werrors -std=c99 -O1
LDLIBS = `pkg-config --cflags --libs gtk+-3.0 sdl SDL_image`

gui: gui.c image_processing.h digitRecog.h network.h solver.h
	$(CC) $(CFLAGS) -o gui gui.c digitRecog.c network.c solver.c image_processing.c $(LDLIBS) -lm -Iutil

image_processing.o: image_processing.c image_processing.h
	$(CC) $(CFLAGS) -c image_processing.c image_processing.h $(LDLIBS)

digitRecog.o: digitRecog.c digitRecog.h network.o
	$(CC) $(CFLAGS) -c digitRecog.c digitRecog.h network.o

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
