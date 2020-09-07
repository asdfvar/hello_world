CC = g++

INC = -I..

LIBDIR = -L..
LIBS = -larithmetic

OBJS = main.o

all: $(OBJS)
	cd .. && make -f arithmetic.mk
	$(CC) $^ $(LIBDIR) $(LIBS) -o main

clean:
	cd .. && make -f arithmetic.mk clean
	rm $(OBJS) main

%.o: %.c
	$(CC) $(INC) $(LIBDIR) $(LIBS) -c $^
