# Rutas de MSYS2 MinGW64 en tu sistema
INCLUDES = -IC:/msys64/mingw64/include
LIBS = -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Regla principal para compilar el juego
all: bin/juego

bin/juego: src/main.cpp
	g++ -B. -Wall -std=c++17 $(INCLUDES) src/main.cpp -o bin/juego $(LIBS)

# Regla para limpiar el ejecutable viejo
clean:
	rm -f bin/juego bin/juego.exe