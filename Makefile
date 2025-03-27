CXX = clang++
CFLAGS = -g -I./inc
LDFLAGS = -lGL -lglfw -lm -ldl -lassimp

SRC_DIR = src
OBJ_DIR = bin

SRC = $(wildcard src/*.cpp)
OBJ = $(subst src,bin,$(SRC:.cpp=.o))

TARGET = test

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*
