# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Project files
SRCS = main.cpp calculator.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = calculator

# Default target
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencies
main.o: main.cpp calculator.hpp
calculator.o: calculator.cpp calculator.hpp

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program with the default input file
run: $(TARGET)
	./$(TARGET) input.txt

.PHONY: all clean run