CXX = g++ -std=c++11
LDFLAGS =

BIN = NOARCH.x

INCLIST =

CXXFLAGS = -O3
DFLAGS =

#
#  targets
#
all:$(BIN)

CXXSRC := $(filter-out main.C, $(wildcard *.C))

CXXOBJ=$(CXXSRC:.C=.o)

OBJS_SRC = $(CXXOBJ)
OBJS := $(notdir $(OBJS_SRC))

$(BIN):  $(OBJS) $(wildcard *.h)
	@echo OBJS = $(OBJS)
	$(CXX) main.C $(wildcard *.h) $(OBJS) $(LDFLAGS) -o $(BIN)

.C.o:
	$(CXX) -o $@ $(CXXFLAGS) $(DFLAGS) -c $(INCLIST) $<

clean:
	rm -f *.o  $(BIN)
