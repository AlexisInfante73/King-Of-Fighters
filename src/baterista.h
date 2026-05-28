#ifndef BATERISTA_H
#define BATERISTA_H

#include "personaje.h"

class Baterista : public Personaje {
private:
    int ritmoBase;

public:
    Baterista(std::string _nombre, std::string _anime, int _moral, int _velocidad, bool _puro, const char* _rutaSprite, int _ritmo)
        : Personaje(_nombre, _anime, _moral, _velocidad, _puro, _rutaSprite), ritmoBase(_ritmo) {}

    void dibujar() override {
        DrawTextureEx(sprite, posicion, 0.0f, 1.0f, WHITE); 

        if (atacando) {
            // Dibuja un círculo semi-transparente azul que simula un escudo de ritmo
            DrawCircleV({posicion.x + (sprite.width/2), posicion.y + (sprite.height/2)}, 85.0f, Fade(BLUE, 0.4f));
            DrawText("🥁 ¡RITMO SÓLIDO! 🛡️", posicion.x - 10, posicion.y - 30, 20, BLUE);
        }
    }
};

#endif