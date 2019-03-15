CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror -std=c++17
LDFLAGS  := -L /usr/local/lib -lboost_program_options -lelf
OBJ_DIR  := ./obj
TARGET   := sim
INCLUDE  := /usr/local/include/boost /usr/local/include/libelf ./
OBJDIRS  := memory infra infra/config infra/elf rf instruction funcsim .
OBJECTS  := $(wildcard $(addsuffix /*.cpp, $(OBJDIRS)))
OBJECTS  := $(OBJECTS:.cpp=.o)

all: $(OBJECTS)
	$(CXX) -o sim $(OBJECTS) $(LDFLAGS)
    
%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(addprefix -I,  $(INCLUDE))
