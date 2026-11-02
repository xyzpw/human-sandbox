GXX = g++
TARGET = sandbox

CXXFLAGS = -fsanitize=address -Wall -std=c++20 -Iinclude
LDFLAGS = -lraylib -lbox2d

SRC = $(shell find . -type f -name '*.cc')
INC = $(shell find . -type f -name '*.hpp')

$(TARGET): $(SRC) $(INC)
	$(GXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)
