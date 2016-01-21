CFLAGS=-g -O2 -Wall


PROGRAM_NAME= ppmf
OBJS = main.o readwriteppm.o processimage.o 
CC = mpicc

$(PROGRAM_NAME): $(OBJS)
	$(CC) -o $@ $? 

clean:
	rm  *.o $(PROGRAM_NAME) *~
