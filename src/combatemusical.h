#ifndef COMBATEMUSICAL_H
#define COMBATEMUSICAL_H

#include <SFML/Graphics.hpp>

// Se define AQUÍ una sola vez para todo el proyecto
enum EstadoPersonaje { IDLE, ATACANDO, ESPECIAL };

class Combatemusical {
private:
    sf::RectangleShape suelo;

public:
    Combatemusical();
    void actualizar();
    void dibujar(sf::RenderWindow& window);
};

#endif // COMBATEMUSICAL_H