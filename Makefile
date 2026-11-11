CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Iinclude
LDFLAGS  ?= -lsfml-graphics -lsfml-window -lsfml-system

TARGET := app
BUILD  := build
SRC    := $(wildcard src/*.cpp)
OBJ    := $(patsubst src/%.cpp,$(BUILD)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

.PHONY: all run clean
