# Compiler and flags
# g++ src/*.cpp  -o main -std=c++11 -Wall -Wno-sign-compare -Wno-format -g -O3 //--Manual run
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wno-sign-compare -Wno-format -g -O3

# Directories
SRCDIR = src
INCDIR = include
BINDIR = .
SOLVERDIR = cdcl

# Source files
SOURCES = $(wildcard $(SRCDIR)/$(SOLVERDIR)/*.cpp)
SELECTED_SOURCES =  $(wildcard $(SRCDIR)/main.cpp) $(wildcard $(SRCDIR)/parseDIMACS.cpp)
OBJ = $(SRC:.cpp=.o)
EXECUTABLE = main

# Build target
all: $(EXECUTABLE)

# Rule to create executable
$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $^
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	del main.exe
	del test.exe

run: $(EXECUTABLE) 
	$(EXECUTABLE) $(arg) $(prepr) $(proof) 
