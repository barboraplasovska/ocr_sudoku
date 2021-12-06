CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1
#LDLIBS = `pkg-config --cflags --libs sdl SDL_image`

main: main.c mnist-stats.o mnist-utils.o screen.o network.o
	$(CC) $(CFLAGS) -o main main.c mnist-stats.o mnist-utils.o screen.o network.o -lm -Iutil

image_processing.o: image_processing.c image_processing.h
	$(CC) $(CFLAGS) -c image_processing.c image_processing.h $(LDLIBS)

mnist-stats.o: mnist-stats.c mnist-stats.h
	$(CC) $(CFLAGS) -c mnist-stats.c mnist-stats.h

mnist-utils.o: mnist-utils.c mnist-utils.h
	$(CC) $(CFLAGS) -c mnist-utils.c mnist-utils.h

screen.o: screen.c screen.h
	$(CC) $(CFLAGS) -c screen.c screen.h

network.o: network.c network.h
	$(CC) $(CFLAGS) -c network.c network.h

clean:
	${RM} -f *.o
	${RM} -f *.gch
	${RM} -f *.out
	${RM} -rf *.dSYM
	${RM} main