CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror -std=c++17
LDFLAGS  := -L /usr/local/lib -lboost_program_options -lelf
ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
OBJ_DIR  := ./obj
TARGET   := sim
INCLUDE  := /usr/local/include/boost /usr/local/include/libelf ./


OBJDIRS  := new_memory infra infra/config infra/elf rf instruction perfsim port
OBJECTS  := $(wildcard $(addsuffix /*.cpp, $(OBJDIRS)))
OBJECTS  := $(OBJECTS:.cpp=.o)
DEPS     := $(OBJECTS:.o=.d)
TESTS    := common instruction
TESTS    := $(addsuffix .run, $(addprefix tests/, $(TESTS)))

all: $(TARGET)

test: $(TESTS)

$(TARGET): $(OBJECTS) main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.run: %.test
	exec $^

%.test: $(OBJECTS) %.o infra/test/test_main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(addprefix -I,  $(INCLUDE))



.PHONY: clean

clean:
	rm -f $(OBJECTS) main.o
	rm -f $(TARGET)
	rm -f $(DEPS) main.d
