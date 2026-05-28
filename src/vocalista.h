#ifndef VOCALISTA_H
#define VOCALISTA_H

#include "personaje.h"

class Vocalista : public Personaje {
private:
    int carisma;

public:
    Vocalista(std::string _nombre, std::string _anime, int _moral, int _velocidad, bool _puro, const char* _rutaSprite, int _carisma)
        : Personaje(_nombre, _anime, _moral, _velocidad, _puro, _rutaSprite), carisma(_carisma) {}

    void dibujar() override {
        // Dibujamos el sprite .png en sus coordenadas X, Y. WHITE significa que mantiene sus colores originales
        DrawTextureEx(sprite, posicion, 0.0f, 1.0f, WHITE); 

        // Si el usuario activó el ataque del vocalista, dibujamos un efecto visual
        if (atacando) {
            // Dibuja círculos concéntricos amarillos simulando ondas de sonido
            DrawCircleLines(posicion.x + (sprite.width / 2), posicion.y + (sprite.height / 2), 70.0f, YELLOW);
            DrawCircleLines(posicion.x + (sprite.width / 2), posicion.y + (sprite.height / 2), 90.0f, GOLD);
            DrawText("🎤 ¡HYPE VOCAL! 🎵", posicion.x, posicion.y - 30, 20, MAROON);
        }
    }
};

#endif