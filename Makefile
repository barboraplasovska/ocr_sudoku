# Makefile

LDFLAGS = 
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1


# SOLVER
solver: solver.c functions.o
	$(CC) $(LDFLAGS) -o solver solver.c functions.o -lm

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c functions.h

# XOR
xor: xor.c network.o layer.o neuron.o
	$(CC) $(LDFLAGS) -o xor xor.c network.o layer.o neuron.o -lm

network.o: network.c 
	$(CC) $(CFLAGS) -c network.c network.h

layer.o: layer.c
	$(CC) $(CFLAGS) -c layer.c layer.h

neuron.o: neuron.c
	$(CC) $(CFLAGS) -c neuron.c neuron.h

# remove object files and executable when user executes "make clean"
clean:
	${RM} -f *.o
	${RM} -f *.gch
	${RM} *.result
	${RM}  xor
	${RM} main
	${RM} solver

# END
