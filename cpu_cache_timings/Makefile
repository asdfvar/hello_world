CC = g++

LIBS = -lpthread -O0

OBJS = main.o  \
       timer.o \
#

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o main

clean:
	rm -v $(OBJS) main

%.o: %.cpp
	$(CC) -c $^
