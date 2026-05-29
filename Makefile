# Variables de configuración
CXX = g++
CXXFLAGS = -Wall -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Carpeta de origen y destino
SRC = src/main.cpp
TARGET = bin/juego

# Regla principal para compilar
all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Regla para limpiar el proyecto
clean:
	rm -rf bin/juego juego