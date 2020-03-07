
MAINSOURCE := test.c
SOURCE  := $(wildcard lib/*.c)
override SOURCE := $(filter-out $(MAINSOURCE),$(SOURCE))
OBJS    := $(patsubst %.c,%.o,$(SOURCE))

TARGET  := WebServer
CC      := cc
LIBS    := -lpthread
INCLUDE:= -I./usr/local/lib
CFLAGS  :=  -g -Wall -O3 -D_PTHREADS
CXXFLAGS:= $(CFLAGS)



.PHONY : objs clean
all : $(TARGET)
objs : $(OBJS)

clean :
	find lib/ -name '*.o' | xargs rm -f
	find . -name $(TARGET) | xargs rm -f
debug:
	@echo $(SOURCE)

$(TARGET) : $(OBJS) test.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
