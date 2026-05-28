#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <string>
#include <raylib.h> // 🌟 Ahora incluimos la librería gráfica

class Personaje {
protected:
    std::string nombre;
    std::string animeOrigen;
    int moral;          
    int velocidadBase;  
    bool esCorazonPuro; 

    // 🎨 Atributos Gráficos Nuevos:
    Texture2D sprite;    // Guarda la imagen .png en la memoria de la tarjeta de video
    Vector2 posicion;    // Guarda las coordenadas X y Y en la pantalla (ej. x=400, y=300)
    bool atacando;       // Si es true, el personaje activará un destello visual

public:
    // El constructor ahora recibe la ruta del archivo de imagen (ej. "assets/luffo.png")
    Personaje(std::string _nombre, std::string _anime, int _moral, int _velocidad, bool _puro, const char* _rutaSprite)
        : nombre(_nombre), animeOrigen(_anime), moral(_moral), velocidadBase(_velocidad), esCorazonPuro(_puro), atacando(false) {
        
        // Cargamos la imagen automáticamente al crear al personaje
        sprite = LoadTexture(_rutaSprite); 
        posicion = { 0.0f, 0.0f }; // Por defecto nacen en la esquina superior izquierda
    }

    // Destructor: En videojuegos es OBLIGATORIO liberar las imágenes de la memoria al cerrar el juego
    virtual ~Personaje() {
        UnloadTexture(sprite); 
    }

    // Getters y Setters
    std::string getNombre() const { return nombre; }
    std::string getAnimeOrigen() const { return animeOrigen; }
    int getMoral() const { return moral; }
    int getVelocidadBase() const { return velocidadBase; }
    bool getEsCorazonPuro() const { return esCorazonPuro; }
    
    void setPosicion(float x, float y) { posicion = { x, y }; }
    void setAtacando(bool estado) { atacando = estado; }

    void reducirMoral(int cantidad) {
        moral -= cantidad;
        if (moral < 0) moral = 0;
    }

    // ✨ El gran cambio polimórfico:
    // En vez de "tocarNota" en texto, ahora cada clase hija tendrá la obligación de "dibujarse" en la ventana gráfica.
    virtual void dibujar() = 0;
};

#endif