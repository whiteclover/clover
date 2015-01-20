
.PHONY: all clean

all: post-build


post-build: main-build

main-build: pre-build
	@$(MAKE) --no-print-directory $(TARGET)

pre-build:
    $(shell protoc -I=. --cpp_out=.  addressbook.proto)


SOURCES = $(wildcard *.c *.cpp *.cc) 

OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(SOURCES))))

TARGET = pb


$(TARGET): $(OBJS)	
	$(CPP)  $(OBJS) -o  $(TARGET)  $(CPPFLAGS)

clean:
	rm -rf *.o $(TARGET) *.pb.*

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp %.cc
	$(CPP) $(CPPFLAGS) -c $< -o $@


CC = gcc
CPP = g++ 

CFLAGS = -Wall -O –g
CPPFLAGS = -std=c++11 -Wall -O -g  -lprotobuf