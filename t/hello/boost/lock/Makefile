OBJS = lockfree.o 
CPP = g++ 

CPPFLAGS = -std=c++11 -Wall -O -g -pthread 

TARGET = lockfree

$(TARGET): $(OBJS)
	$(CPP)  $(OBJS) -o  $(TARGET)  $(CPPFLAGS)

lockfree.o: lockfree.cpp
	$(CPP) -c lockfree.cpp -o lockfree.o  $(CPPFLAGS)

clean:
	rm -rf *.o $(TARGET)