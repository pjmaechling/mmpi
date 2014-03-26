CC = mpicc

OPTFLAGS = -O1
CFLAGS = $(OPTFLAGS)

all:mmpi

mmpi:mmpi.c
	$(CC) $(CFLAGS) -o mmpi mmpi.c -lm

clean:
	rm *.o mmpi *.txt
