#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include "personaje.h"
#include <vector>
#include <iostream>
#include <cstdlib>

struct Nota {
    sf::RectangleShape cuerpo;
    int carril;
    bool activa;

    Nota(int c, float posX) {
        carril = c;
        activa = true;
        cuerpo.setSize(sf::Vector2f(60, 20));
        cuerpo.setFillColor(sf::Color::Cyan);
        cuerpo.setPosition(posX, 0);
    }
};

int main() {
    // 1. CREACIÓN DE LA VENTANA (1280x720)
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Anime Rhythm Battle - Osu! Mania Mod");
    window.setFramerateLimit(60);

    // Variables de estado del juego
    int estadoJuego = 0; 
    float carrilX[4] = { 500, 570, 640, 710 };
    std::vector<Nota> listaNotas;

    (void)estadoJuego;
    (void)carrilX;

    // ========================================================
    // 2. CREACIÓN DE LOS 6 PERSONAJES USANDO PUNTEROS
    // ========================================================
    std::vector<Personaje*> banda;

    // Se crean de manera segura usando 'new Personaje(...)'
    banda.push_back(new Personaje("Naruto", "assets/images/naruto.png", "assets/sounds/textures/musica_naruto.ogg", sf::Color::Yellow, 300));
    banda.push_back(new Personaje("Sasuke", "assets/images/sasuke.png", "assets/sounds/textures/musica_sasuke.ogg", sf::Color::Blue, 0));
    banda.push_back(new Personaje("Kakashi", "assets/images/kakashi.png", "assets/sounds/textures/musica_kakashi.ogg", sf::Color::Green, 0));
    banda.push_back(new Personaje("Sakura", "assets/images/sakura.png", "assets/sounds/textures/musica_sakura.ogg", sf::Color::Magenta, 0));
    banda.push_back(new Personaje("Itachi", "assets/images/itachi.png", "assets/sounds/textures/musica_itachi.ogg", sf::Color::Red, 0));
    banda.push_back(new Personaje("Gaara", "assets/images/gaara.png", "assets/sounds/textures/musica_gaara.ogg", sf::Color::Cyan, 0));

    // Al iniciar el juego, se reproduce la pista del primer personaje (Naruto)
    banda[0]->reproducirMusica();

    // ========================================================
    // 3. BUCLE PRINCIPAL DEL JUEGO
    // ========================================================
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // INTERACCIÓN: Cambiar canción presionando los números del 1 al 6
            if (event.type == sf::Event::EventType::KeyPressed) {
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num6) {
                    int personajeElegido = event.key.code - sf::Keyboard::Num1;

                    // 1. Apagamos la música de todos los personajes para que no se empalmen
                    for (size_t i = 0; i < banda.size(); i++) {
                        banda[i]->detenerMusica();
                    }

                    // 2. Encendemos la música del elegido (¡Ya corregida la variable aquí!)
                    banda[personajeElegido]->reproducirMusica();
                    std::cout << "Cambiando a la musica de: " << banda[personajeElegido]->nombre << std::endl;
                }
            }
        }

        // ========================================================
        // 4. ACTUALIZAR LAS ANIMACIONES DE LA BANDA (LÓGICA)
        // ========================================================
        for (size_t i = 0; i < banda.size(); i++) {
            banda[i]->actualizarAnimacion();
        }

        // Lógica de caída para las notas musicales
        for (size_t i = 0; i < listaNotas.size(); i++) {
            if (listaNotas[i].activa) {
                listaNotas[i].cuerpo.move(0, 5); 
            }
        }

        // ========================================================
        // 5. RENDERIZADO (DIBUJAR EN PANTALLA)
        // ========================================================
        window.clear(sf::Color(25, 25, 25)); 

        // Dibuja los personajes alineados horizontalmente uno al lado del otro
        for (size_t i = 0; i < banda.size(); i++) {
            banda[i]->dibujar(window, i * 180 + 50, 350);
        }

        // Dibuja las notas rítmicas por encima
        for (size_t i = 0; i < listaNotas.size(); i++) {
            if (listaNotas[i].activa) {
                window.draw(listaNotas[i].cuerpo);
            }
        }

        window.display();
    }

    // Al cerrar el juego apagamos la música y limpiamos la memoria
    for (size_t i = 0; i < banda.size(); i++) {
        banda[i]->detenerMusica();
        delete banda[i]; 
    }

    return 0;
}