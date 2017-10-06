CC=gcc -Wall -fopenmp -lpthread
CINCL=common/*.c

all:
	$(CC) -o client.out client.c unix/uclient.c $(CINCL) -lm
	$(CC) -o unix/userver.out unix/userver.c $(CINCL) -lm

clean:
	rm -f client.out unix/userver.out
