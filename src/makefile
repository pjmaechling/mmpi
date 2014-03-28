##
## Mira uses mpicc to wrap MPI libraries
## Blue Waters uses cc to wrap MPI libraries
CC = mpicc

OPTFLAGS = -O1
CFLAGS = $(OPTFLAGS)

all:mmpi

mmpi:mmpi.c
	$(CC) $(CFLAGS) -o mmpi mmpi.c -lm

clean:
	rm *.o mmpi
