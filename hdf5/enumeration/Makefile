CC = g++

LIB_DIR = -L/home/euler/user_libs/hdf5/hdf5_1p12p1/lib/

INC = -I/home/euler/user_libs/hdf5/hdf5_1p12p1/include/

LIBS = /home/euler/user_libs/hdf5/hdf5_1p12p1/lib/libhdf5.a -ldl -lz 

OBJS = enumeration.o

all: $(OBJS)
	$(CC) $^ $(INC) $(LIB_DIR) $(LIBS) -o test

clean:
	rm -f $(OBJS) test enumeration.h5

%.o: %.c
	$(CC) $^ $(INC) -c
