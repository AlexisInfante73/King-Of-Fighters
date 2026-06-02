#include "combatemusical.h"
#include <SFML/Graphics.hpp>

// Constructor de la clase - Configura el escenario base
Combatemusical::Combatemusical() {
    suelo.setSize(sf::Vector2f(800.f, 150.f));
    suelo.setFillColor(sf::Color(50, 50, 50)); // Gris oscuro estilo KOF
    suelo.setPosition(0.f, 450.f);             // Posicionado al fondo
}

void Combatemusical::actualizar() {
    // Lógica futura de actualización rítmica
}

void Combatemusical::dibujar(sf::RenderWindow& window) {
    // Dibuja el suelo gris en la ventana principal
    window.draw(suelo);
}