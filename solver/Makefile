LDFLAGS =
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1

solver: main.c functions.c
	$(CC) $(LDFLAGS) -o solver main.c functions.c -lm

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c functions.h

# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o
	rm -f *.gch
	rm  solver
	rm  *.result
