CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror -std=c++17
LDFLAGS  := -L /usr/local/lib -lboost_program_options -lelf
OBJ_DIR  := ./obj
TARGET   := sim
INCLUDE  := /usr/local/include/boost /usr/local/include/libelf ./

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(addprefix -I,  $(INCLUDE))
