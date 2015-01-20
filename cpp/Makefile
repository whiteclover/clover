SRC_DIR = ./clover

VPATH = $(SRC_DIR)  $(SRC_DIR)/Threading $(SRC_DIR)/Network $(SRC_DIR)/Base $(SRC_DIR)/Http

SRC_ROOT = $(subst $(SRC_DIR)/,, $(wildcard $(SRC_DIR)/*.cpp) )
BASE = $(subst $(SRC_DIR)/BASE/,, $(wildcard $(SRC_DIR)/BASE/*.cpp) )
THREADING = $(subst $(SRC_DIR)/Threading/,, $(wildcard $(SRC_DIR)/Threading/*.cpp) )
NETWORK = $(subst $(SRC_DIR)/Network/,, $(wildcard $(SRC_DIR)/Network/*.cpp) )
HTTP = $(subst $(SRC_DIR)/Http/,, $(wildcard $(SRC_DIR)/Http/*.cpp) )

SRC = $(SRC_ROOT) $(BASE) $(THREADING) $(NETWORK) $(HTTP)

CPP_SRCS = $(SRC)

INCS = -I$(SRC_DIR)/Network -I$(SRC_DIR)/Base -I$(SRC_DIR)/Threading -I$(SRC_DIR)/Http

LIBS = -lpthread -levent -lprotobuf -levent_pthreads -lpcre


TARGET = cloverd


$(TARGET) : $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o) 
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIBS) $(INCS)



CC = gcc
CPP = g++
CFLAGS = -Wall -g -O0 -Wno-deprecated -Wno-write-strings 
CPPFLAGS = -std=c++11 -Wall -g -O0 -Wno-deprecated -Wno-write-strings  -fpermissive


%.o: %.c
	$(CC) $(CFLAGS) $(INCS) $(MACROS) -c $<


%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INCS) $(MACROS) -c $<


%.d: %.c
	@set -e; rm -f $@; \
		$(CC) -MM $(CFLAGS) $(INCS) $(MACROS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$


%.d: %.cpp
	@set -e; rm -f $@; \
		$(CPP) -MM $(CPPFLAGS) $(INCS) $(MACROS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

clean: $(DIRS)
	$(RM) *.o *.d *.d.* $(TARGET)


-include $(CPP_SRCS:.cpp=.d) $(C_SRCS:.c=.d)