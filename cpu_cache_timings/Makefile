CC = g++

LIBS = -lpthread

OBJS = main.o  \
       timer.o \
#

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o main

clean:
	rm -v $(OBJS) main

%.o: %.cpp
	$(CC) -c $^
