TARGET = poly

SRC = main.cpp generator.cpp

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
