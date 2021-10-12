LDFLAGS = 
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1

main: main.c network.o layer.o neuron.o
	$(CC) $(LDFLAGS) -o main main.c network.o layer.o neuron.o -lm

network.o: network.c 
	$(CC) $(CFLAGS) -c network.c network.h

layer.o: layer.c
	$(CC) $(CFLAGS) -c layer.c layer.h

neuron.o: neuron.c
	$(CC) $(CFLAGS) -c neuron.c neuron.h

# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o
	rm -f *.gch
	rm  main
