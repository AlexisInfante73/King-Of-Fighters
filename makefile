# Makefile actualizado para soportar Raylib en Linux
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Isrc

# 🌟 Banderas especiales para conectar la librería gráfica y el sistema de audio
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/juego

all: $(TARGET)

$(TARGET): $(SRC_DIR)/main.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/main.cpp -o $(TARGET) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -rf $(BIN_DIR)