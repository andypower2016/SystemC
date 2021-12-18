CC      := gcc
CXX     := g++
INC     := -I"$(SYSTEMC_HOME)/include" 
CFLAGS  := -O3 -Wall -pthread $(INC)

SRC 	:= ./
SRCS    := $(shell find $(SRC) -name '*.cpp')

OBJS    := $(SRCS:%.cpp=%.o)

BIN 	:= ./
LDFLAGS := -L"$(SYSTEMC_HOME)/lib-linux64"
LDLIBS	:= -lm -lsystemc
MKDIR   := mkdir
RMDIR   := rmdir

.PHONY: all
all:$(OBJS) | $(BIN)
	$(CXX) -o $(BIN)/main $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	@echo make target [$@] is complete


%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)
	@echo make target [$@] is complete

$(BIN):
	$(MKDIR) $(BIN)

.PHONY: clean
clean:
	-rm -f $(BIN)/main $(OBJS)



