#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <string>
#include <iostream>

class Personaje {
public:
    std::string nombre;
    sf::Texture textura;
    sf::Sprite sprite;
    sf::RectangleShape cajaAlternativa; 
    int vidaActual;
    int vidaMaxima;
    bool tieneImagen;
    bool tieneMusica; 

    sf::Clock relojAnimacion;
    int cuadroActual;
    int totalCuadros;  
    int anchoCuadro;   
    int altoCuadro;    
    int filaActual;    

    sf::Music musica;  

    Personaje(std::string name, std::string rutaImagen, std::string rutaMusica, sf::Color colorRespaldo, int filaCorte = 0) {
        nombre = name;
        vidaMaxima = 100;
        vidaActual = 100;
        tieneImagen = false;
        tieneMusica = false;

        cuadroActual = 0;
        totalCuadros = 5;   
        anchoCuadro = 204;  
        altoCuadro = 300;   
        filaActual = filaCorte; 

        if (textura.loadFromFile(rutaImagen)) {
            sprite.setTexture(textura);
            sprite.setTextureRect(sf::IntRect(0, filaActual, anchoCuadro, altoCuadro));
            tieneImagen = true;
        } else {
            std::cout << "[AVISO]: No se encontro la imagen para " << nombre << ". Usando caja de color." << std::endl;
            cajaAlternativa.setSize(sf::Vector2f(100, 100));
            cajaAlternativa.setFillColor(colorRespaldo);
            cajaAlternativa.setOutlineThickness(3);
            cajaAlternativa.setOutlineColor(sf::Color::White);
        }

        if (!rutaMusica.empty()) {
            if (musica.openFromFile(rutaMusica)) {
                musica.setLoop(true); 
                tieneMusica = true;
            } else {
                std::cout << "[AVISO]: No se pudo cargar la musica para " << nombre << std::endl;
            }
        }
    }

    void actualizarAnimacion() {
        if (!tieneImagen) return; 

        if (relojAnimacion.getElapsedTime().asSeconds() >= 0.15f) {
            cuadroActual = (cuadroActual + 1) % totalCuadros;
            sprite.setTextureRect(sf::IntRect(cuadroActual * anchoCuadro, filaActual, anchoCuadro, altoCuadro));
            relojAnimacion.restart(); 
        }
    }

    void reproducirMusica() {
        if (tieneMusica) {
            musica.play();
        }
    }

    void detenerMusica() {
        if (tieneMusica) {
            musica.stop();
        }
    }

    void dibujar(sf::RenderWindow& ventana, float x, float y) {
        if (tieneImagen) {
            sprite.setPosition(x, y);
            ventana.draw(sprite);
        } else {
            cajaAlternativa.setPosition(x, y);
            ventana.draw(cajaAlternativa);
        }
    }

    void recibirDanio(int cantidad) {
        vidaActual -= cantidad;
        if (vidaActual < 0) vidaActual = 0;
    }
};

#endif