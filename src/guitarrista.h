#ifndef GUITARRISTA_H
#define GUITARRISTA_H

#include "personaje.h"

class Guitarrista : public Personaje {
private:
    int velocidadDedos;

public:
    Guitarrista(std::string _nombre, std::string _anime, int _moral, int _velocidad, bool _puro, const char* _rutaSprite, int _dedos)
        : Personaje(_nombre, _anime, _moral, _velocidad, _puro, _rutaSprite), velocidadDedos(_dedos) {}

    void dibujar() override {
        DrawTextureEx(sprite, posicion, 0.0f, 1.0f, WHITE); 

        if (atacando) {
            // Dibuja un rectángulo de líneas rojas alrededor del guitarrista simulando distorsión
            DrawRectangleLines(posicion.x - 10, posicion.y - 10, sprite.width + 20, sprite.height + 20, RED);
            DrawText("🎸 ¡SOLO LEGENDARIO! 🔥", posicion.x - 20, posicion.y - 30, 20, ORANGE);
        }
    }
};

#endif