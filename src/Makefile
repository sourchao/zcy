#common makefile header

CC = g++

DIR_INC = ../include
DIR_BIN = ../bin
DIR_LIB = ../libs

TARGET	= sr_baseIflytek
BIN_TARGET = $(DIR_BIN)/$(TARGET)

CROSS_COMPILE = 
ifdef DEBUG
CFLAGS = -g -Wl,--no-as-needed -std=c++11 -Wall -I$(DIR_INC) -DDEBUG
else
CFLAGS = -g -Wl,--no-as-needed -std=c++11 -Wall -I$(DIR_INC)
endif

ifdef LINUX64
LDFLAGS := -L$(DIR_LIB)/x64
else
LDFLAGS := -L$(DIR_LIB)/x86 
endif
LDFLAGS += -lmsc -lrt -ldl -lpthread

OBJECTS := $(patsubst %.cc,%.o,$(wildcard *.cc))

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.cc
	$(CROSS_COMPILE)$(CC) -c $(CFLAGS) $< -o $@

clean:
	@rm -f *.o $(BIN_TARGET)

.PHONY:clean

#common makefile foot
