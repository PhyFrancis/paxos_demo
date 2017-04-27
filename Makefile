CXX = g++
LDFLAGS =

BIN = NOARCH.x

INCLIST =

CXXFLAGS = -O3
DFLAGS =

#
#  targets
#
all:$(BIN)

.SUFFIXES:  .o .C

CXXSRC :=$(wildcard *.C)

CXXOBJ=$(CXXSRC:.C=.o)

OBJS_SRC = $(CXXOBJ)
OBJS := $(notdir $(OBJS_SRC))

$(BIN):  $(OBJS)
	@echo OBJS = $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(BIN)

.C.o:
	$(CXX) -o $@ $(CXXFLAGS) $(DFLAGS) -c $(INCLIST) $<

clean:
	rm -f *.o  $(BIN)
