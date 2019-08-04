CC = g++

OBJS = c2py.o \
#

LIBS = -pthread -lpython2.7

INC = -I/usr/include/python2.7

EXE = c2py

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(INC) -o $(EXE)

clean:
	rm -v $(OBJS) $(EXE)

%.o: %.cpp
	$(CC) $^ $(INC) -c
